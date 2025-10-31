#include "automaton/app/canvas.hpp"

#include "automaton/app/controller.hpp"
#include "automaton/core/defaults.hpp"
#include "automaton/core/grid.hpp"

#include <cairomm/fontface.h>
#include <gdkmm/general.h>
#include <gdkmm/rgba.h>
#include <glibmm/main.h>

#include <algorithm>
#include <format>
#include <memory>
#include <string>
#include <vector>

namespace automaton::app {

namespace palette {
static const Gdk::RGBA font{defaults::palette::font};
static const Gdk::RGBA border{defaults::palette::border};
static const Gdk::RGBA background{defaults::palette::background};
static const std::vector<Gdk::RGBA> states{defaults::palette::states.begin(), defaults::palette::states.end()};
};  // namespace palette

canvas::canvas(const core::grid& grid)
    : _grid(grid) {
    // catch mouse press/release events
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    signal_button_press_event().connect(sigc::mem_fun(*this, &canvas::_on_mouse_press));
    signal_button_release_event().connect(sigc::mem_fun(*this, &canvas::_on_mouse_release));

    // catch mouse motion event
    add_events(Gdk::POINTER_MOTION_MASK);
    signal_motion_notify_event().connect(sigc::mem_fun(*this, &canvas::_on_mouse_motion));

    // catch mouse scroll event
    add_events(Gdk::SCROLL_MASK);
    signal_scroll_event().connect(sigc::mem_fun(*this, &canvas::_on_mouse_scroll));

    // catch keyboard events
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;
    signal_key_press_event().connect(sigc::mem_fun(*this, &canvas::_on_key_press));

    // set timeout to redraw
    _redraw_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &canvas::_on_redraw_timeout),
                                                        defaults::window::redraw_delay_ms);

    // add draw function
    signal_draw().connect(sigc::mem_fun(*this, &canvas::_on_draw));
}

void canvas::initialize(canvas_config cfg, std::shared_ptr<controller> ctrl) {
    _cfg = cfg;
    _ctrl = ctrl;

    if (!_cfg.adapt_to_window) _ctrl->engine_resize(_cfg.initial_size);
}

void canvas::on_resize() {
    g_debug("canvas::on_resize()");
    if (_cfg.adapt_to_window && _ctrl) _ctrl->engine_resize(_calculate_grid_size());
}

bool canvas::_on_key_press(GdkEventKey* ev) {
    g_debug("canvas::on_key_press(key='%s')", ev->string);

    if (ev->keyval == GDK_KEY_space) {
        _ctrl->animation_toggle();
    } else if (ev->keyval == GDK_KEY_s) {
        _ctrl->engine_step();
    } else if (ev->keyval == GDK_KEY_c) {
        _ctrl->engine_clear();
    } else if (ev->keyval == GDK_KEY_r) {
        _ctrl->engine_restart();
    } else if (ev->keyval == GDK_KEY_Tab) {
        _ctrl->engine_shift_actions();
    } else if (ev->keyval == GDK_KEY_i) {
        _cfg.show_status = !_cfg.show_status;
    } else {
        return false;
    }

    return true;
}

bool canvas::_on_mouse_press(GdkEventButton* ev) {
    g_debug("canvas::on_mouse_press(button=%d,type=%d)", ev->button, ev->type);

    if (ev->button == 1 && ev->type == GDK_BUTTON_PRESS) {  // left click
        _mouse_mode = mouse_modes::DRAW;
        if (!_handle_cell_press(ev->x, ev->y)) return false;

    } else if (ev->button == 3 && ev->type == GDK_BUTTON_PRESS) {  // right click
        _mouse_mode = mouse_modes::ERASE;
        if (!_handle_cell_press(ev->x, ev->y)) return false;

    } else if (ev->button == 2) {  // middle click
        if (ev->type == GDK_BUTTON_PRESS && !_cfg.adapt_to_window) {
            _shift_start = {ev->x, ev->y};
        } else if (ev->type == GDK_DOUBLE_BUTTON_PRESS) {
            _ctrl->animation_toggle();
        }

    } else {
        return false;
    }

    return true;
}

bool canvas::_on_mouse_release(GdkEventButton* ev) {
    g_debug("canvas::on_mouse_release()");

    if ((ev->button == 1 && _mouse_mode == mouse_modes::DRAW) ||
        (ev->button == 3 && _mouse_mode == mouse_modes::ERASE)) {
        _mouse_mode = mouse_modes::NONE;
        return true;
    } else if (ev->button == 2 && _shift_start.has_value()) {
        _shift_start = std::nullopt;
    }

    return false;
}

bool canvas::_on_mouse_motion(GdkEventMotion* ev) {
    /* g_debug("canvas::on_mouse_motion()"); */

    if (_mouse_mode == mouse_modes::DRAW || _mouse_mode == mouse_modes::ERASE) {
        if (!_handle_cell_press(ev->x, ev->y)) return false;
        return true;
    } else if (_shift_start.has_value()) {
        _field_shift.x += ev->x - _shift_start->x;
        _field_shift.y += ev->y - _shift_start->y;
        _shift_start = {ev->x, ev->y};
        return true;
    }

    return false;
}

bool canvas::_on_mouse_scroll(GdkEventScroll* ev) {
    g_debug("canvas::on_mouse_scroll(%d)", ev->direction);

    using namespace defaults;

    if (ev->state & GdkModifierType::GDK_CONTROL_MASK && !_cfg.adapt_to_window) {
        // change grid size
        core::dims size = _grid.size();
        if (ev->direction == GdkScrollDirection::GDK_SCROLL_UP) {
            size.rows++;
            size.cols++;
        } else if (ev->direction == GdkScrollDirection::GDK_SCROLL_DOWN) {
            size.rows--;
            size.cols--;
        }
        _ctrl->engine_resize(size);
    } else if (ev->state & GdkModifierType::GDK_SHIFT_MASK) {
        // change cell width slowly (with addition)
        if (ev->direction == GdkScrollDirection::GDK_SCROLL_UP) {
            _cfg.cell_width = std::min(_cfg.cell_width + cell::width_delta, cell::max_width);
        } else if (ev->direction == GdkScrollDirection::GDK_SCROLL_DOWN) {
            _cfg.cell_width = std::max(_cfg.cell_width - cell::width_delta, cell::min_width);
        }
    } else {
        // change cell width quickly (with multiplication)
        if (ev->direction == GdkScrollDirection::GDK_SCROLL_UP) {
            _cfg.cell_width = std::min(_cfg.cell_width * cell::scale_factor, cell::max_width);
        } else if (ev->direction == GdkScrollDirection::GDK_SCROLL_DOWN) {
            _cfg.cell_width = std::max(_cfg.cell_width / cell::scale_factor, cell::min_width);
        }
        _cfg.cell_width = std::round(_cfg.cell_width * cell::precision) / cell::precision;
    }

    if (_cfg.adapt_to_window) _ctrl->engine_resize(_calculate_grid_size());

    return false;
}

bool canvas::_on_draw(const cairo_context& cr) {
    const core::dims& size = _grid.size();

    const vec2 real_size = _calculate_real_size();
    const vec2 field_start = _calculate_field_start(real_size);

    _draw_background(cr);
    _draw_frame(cr, real_size, field_start);
    _draw_field(cr, size, field_start);
    if (_cfg.show_borders) _draw_borders(cr, size, real_size, field_start);
    if (_cfg.show_status) _draw_status(cr);

    return false;
}

void canvas::_draw_background(const cairo_context& cr) const {
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, palette::background);
    cr->paint();

    cr->restore();
}

void canvas::_draw_frame(const cairo_context& cr, const vec2& real_size, const vec2& field_start) const {
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, palette::border);
    cr->set_line_width(defaults::cell::border_width);

    cr->rectangle(field_start.x, field_start.y, real_size.x, real_size.y);

    cr->stroke();

    cr->restore();
}

// cell {0,0} is at the top-left corner of the screen
void canvas::_draw_field(const cairo_context& cr, const core::dims& size, const vec2& field_start) const {
    cr->save();

    const auto draw_cells_with_state = [&](uint8_t cell_state, const Gdk::RGBA& color) {
        Gdk::Cairo::set_source_rgba(cr, color);
        for (size_t row = 0; row < size.rows; ++row) {
            for (size_t col = 0; col < size.cols; ++col) {
                if (_grid[row, col] == cell_state) {
                    cr->rectangle(field_start.x + _cfg.cell_width * col,  //
                                  field_start.y + _cfg.cell_width * row,  //
                                  _cfg.cell_width, _cfg.cell_width);
                }
            }
        }
        cr->fill();
    };

    for (size_t i = 0; i < palette::states.size(); ++i)
        draw_cells_with_state(i, palette::states[i]);

    cr->restore();
}

void canvas::_draw_borders(const cairo_context& cr, const core::dims& size, const vec2& real_size,
                           const vec2& field_start) const {
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, palette::border);
    cr->set_line_width(defaults::cell::border_width);

    double x = _cfg.cell_width, y = _cfg.cell_width;

    for (size_t col = 0; col < size.cols - 1; col++) {
        cr->move_to(field_start.x + x, field_start.y + 0);
        cr->line_to(field_start.x + x, field_start.y + real_size.y);

        x += _cfg.cell_width;
    }
    for (size_t row = 0; row < size.rows - 1; row++) {
        cr->move_to(field_start.x + 0, field_start.y + y);
        cr->line_to(field_start.x + real_size.x, field_start.y + y);

        y += _cfg.cell_width;
    }

    cr->stroke();

    cr->restore();
}

void canvas::_draw_status(const cairo_context& cr) const {
    cr->save();

    cr->move_to(defaults::font::margin, defaults::font::margin + defaults::font::size);

    Gdk::Cairo::set_source_rgba(cr, palette::font);
    cr->set_font_size(defaults::font::size);
    cr->select_font_face("", Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_NORMAL);
    cr->show_text(std::format("{}, cell_width={:.3f}, mouse_mode={}", _ctrl->get_status(), _cfg.cell_width,
                              std::to_underlying(_mouse_mode)));

    cr->restore();
}

bool canvas::_on_redraw_timeout() {
    /* g_debug("canvas::on_redraw_timeout()"); */

    if (_mouse_mode == mouse_modes::DRAW || _mouse_mode == mouse_modes::ERASE) {
        int x, y;
        get_pointer(x, y);
        _handle_cell_press(x, y);
    }

    queue_draw();
    return true;
}

core::dims canvas::_calculate_grid_size() const {
    return {static_cast<size_t>(get_height() / _cfg.cell_width),  //
            static_cast<size_t>(get_width() / _cfg.cell_width)};
}

auto canvas::_calculate_real_size() const -> vec2 {
    const core::dims& size = _grid.size();
    return {_cfg.cell_width * size.cols, _cfg.cell_width * size.rows};
}

auto canvas::_calculate_field_start(const vec2& real_size) const -> vec2 {
    return {get_width() / 2.0 + _field_shift.x - real_size.x / 2,
            get_height() / 2.0 + _field_shift.y - real_size.y / 2};
}

bool canvas::_handle_cell_press(int x, int y) {
    const vec2 field_start = _calculate_field_start(_calculate_real_size());

    // NOTE: negative row/col values underflow and don't pass the check below
    size_t col = (x - field_start.x) / _cfg.cell_width;
    size_t row = (y - field_start.y) / _cfg.cell_width;

    const core::dims& size = _grid.size();
    if (row >= size.rows || col >= size.cols) return false;

    if (_mouse_mode == mouse_modes::DRAW) {
        _ctrl->engine_action1(row, col);
    } else if (_mouse_mode == mouse_modes::ERASE) {
        _ctrl->engine_action2(row, col);
    }

    return true;
}

}  // namespace automaton::app
