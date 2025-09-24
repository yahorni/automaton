#include "automaton/app/canvas.hpp"

#include "automaton/app/controller.hpp"
#include "automaton/core/defaults.hpp"
#include "automaton/core/grid.hpp"
#include "glib.h"

#include <cairomm/fontface.h>
#include <gdk/gdk.h>
#include <gdkmm/general.h>
#include <gdkmm/rgba.h>
#include <glibmm/main.h>

#include <memory>
#include <string>

namespace automaton::app {

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
    _redraw_connection =
        Glib::signal_timeout().connect(sigc::mem_fun(*this, &canvas::_on_redraw_timeout), defaults::redraw_timeout_ms);

    // add draw function
    signal_draw().connect(sigc::mem_fun(*this, &canvas::_on_draw));
}

void canvas::initialize(canvas_config cfg, std::weak_ptr<controller> ctrl) {
    _cfg = cfg;
    _ctrl = ctrl;
}

void canvas::on_resize() {
    g_debug("canvas::on_resize()");
    if (_cfg.adapt_to_window) _resize_grid();
}

bool canvas::_on_key_press(GdkEventKey* ev) {
    g_debug("canvas::on_key_press(key='%s')", ev->string);

    if (ev->keyval == GDK_KEY_space) {
        _ctrl.lock()->animation_toggle();
    } else if (ev->keyval == GDK_KEY_s) {
        _ctrl.lock()->engine_step();
    } else if (ev->keyval == GDK_KEY_c) {
        _ctrl.lock()->engine_clear();
    } else if (ev->keyval == GDK_KEY_r) {
        _ctrl.lock()->engine_restart();
    } else if (ev->keyval == GDK_KEY_Tab) {
        _ctrl.lock()->engine_shift_actions();
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
            _mouse_mode = mouse_modes::SHIFT;
            _last_shift_start = {ev->x, ev->y};
        } else if (ev->type == GDK_DOUBLE_BUTTON_PRESS) {
            auto ctrl = _ctrl.lock();
            if (!ctrl) return true;
            ctrl->animation_toggle();
        }

    } else {
        return false;
    }

    return true;
}

bool canvas::_on_mouse_release(GdkEventButton* ev) {
    g_debug("canvas::on_mouse_release()");

    if ((ev->button == 1 && _mouse_mode == mouse_modes::DRAW) ||
        (ev->button == 2 && _mouse_mode == mouse_modes::SHIFT) ||
        (ev->button == 3 && _mouse_mode == mouse_modes::ERASE)) {
        _mouse_mode = mouse_modes::NONE;
        if (_mouse_mode == mouse_modes::SHIFT) {
            _last_shift_start = {0, 0};
        }
        return true;
    }

    return false;
}

bool canvas::_on_mouse_motion(GdkEventMotion* ev) {
    /* g_debug("canvas::on_mouse_motion()"); */

    if (_mouse_mode == mouse_modes::DRAW || _mouse_mode == mouse_modes::ERASE) {
        if (!_handle_cell_press(ev->x, ev->y)) return false;
        return true;
    } else if (_mouse_mode == mouse_modes::SHIFT) {
        _field_shift.x += ev->x - _last_shift_start.x;
        _field_shift.y += ev->y - _last_shift_start.y;
        _last_shift_start = {ev->x, ev->y};
        return true;
    }

    return false;
}

bool canvas::_on_mouse_scroll(GdkEventScroll* ev) {
    g_debug("canvas::on_mouse_scroll(%d)", ev->direction);

    if (ev->state & GdkModifierType::GDK_SHIFT_MASK && !_cfg.adapt_to_window) {
        // change grid size
        core::dims size = _grid.size();
        auto ctrl = _ctrl.lock();
        if (ev->direction == GdkScrollDirection::GDK_SCROLL_UP) {
            size.rows++;
            size.cols++;
        } else if (ev->direction == GdkScrollDirection::GDK_SCROLL_DOWN) {
            size.rows--;
            size.cols--;
        }
        ctrl->engine_resize(size);
    } else {
        // change cell width
        if (ev->direction == GdkScrollDirection::GDK_SCROLL_UP) {
            _cfg.cell_width = std::min(_cfg.cell_width * defaults::scale_factor, defaults::max_cell_width);
        } else if (ev->direction == GdkScrollDirection::GDK_SCROLL_DOWN) {
            _cfg.cell_width = std::max(_cfg.cell_width / defaults::scale_factor, defaults::min_cell_width);
        }
    }

    if (_cfg.adapt_to_window) _resize_grid();

    return false;
}

bool canvas::_on_draw(const cairo_context& cr) {
    const core::dims& size = _grid.size();
    const core::grid_state& state = _grid.state();

    _draw_background(cr);
    _draw_frame(cr, size);
    _draw_field(cr, size, state);
    if (_cfg.show_borders) _draw_borders(cr, size);
    if (_cfg.show_status) _draw_status(cr);

    return false;
}

void canvas::_draw_background(const cairo_context& cr) {
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, _palette.surface_bg);
    cr->paint();

    cr->restore();
}

void canvas::_draw_frame(const cairo_context& cr, const core::dims& size) {
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, _palette.border);
    cr->set_line_width(defaults::cell_border_width);
    _draw_shifted_rectangle(cr, 0, 0, _cfg.cell_width * size.cols, _cfg.cell_width * size.rows);
    cr->stroke();

    cr->restore();
}

// cell {0,0} is at the top-left corner of the screen
void canvas::_draw_field(const cairo_context& cr, const core::dims& size, const core::grid_state& state) {
    cr->save();

    const auto draw_cells_with_state = [&](std::uint8_t cell_state, const Gdk::RGBA& color) {
        Gdk::Cairo::set_source_rgba(cr, color);
        for (size_t row = 0; row < size.rows; ++row) {
            for (size_t col = 0; col < size.cols; ++col) {
                if (state[row][col] == cell_state) {
                    _draw_shifted_rectangle(cr,                     //
                                            _cfg.cell_width * col,  //
                                            _cfg.cell_width * row,  //
                                            _cfg.cell_width,        //
                                            _cfg.cell_width);
                }
            }
        }
        cr->fill();
    };

    draw_cells_with_state(1, _palette.cell_state1);
    draw_cells_with_state(2, _palette.cell_state2);
    draw_cells_with_state(3, _palette.cell_state3);

    cr->restore();
}

void canvas::_draw_borders(const cairo_context& cr, const core::dims& size) {
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, _palette.border);
    cr->set_line_width(defaults::cell_border_width);

    size_t width = _cfg.cell_width * size.cols, height = _cfg.cell_width * size.rows;
    double x = _cfg.cell_width, y = _cfg.cell_width;

    for (size_t col = 0; col < size.cols - 1; col++) {
        _draw_shifted_line(cr, x, 0, x, height);
        x += _cfg.cell_width;
    }
    for (size_t row = 0; row < size.rows - 1; row++) {
        _draw_shifted_line(cr, 0, y, width, y);
        y += _cfg.cell_width;
    }

    cr->stroke();

    cr->restore();
}

void canvas::_draw_status(const cairo_context& cr) {
    cr->save();

    cr->move_to(defaults::font_margin, defaults::font_margin + defaults::font_size);

    Gdk::Cairo::set_source_rgba(cr, _palette.surface_fg);
    cr->set_font_size(defaults::font_size);
    cr->select_font_face("", Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_NORMAL);
    cr->show_text(_ctrl.lock()->get_status());

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

void canvas::_draw_shifted_rectangle(const cairo_context& cr, double x, double y, double width, double height) {
    cr->rectangle(_field_shift.x + x, _field_shift.y + y, width, height);
}

void canvas::_draw_shifted_line(const cairo_context& cr, double x_from, double y_from, double x_to, double y_to) {
    cr->move_to(_field_shift.x + x_from, _field_shift.y + y_from);
    cr->line_to(_field_shift.x + x_to, _field_shift.y + y_to);
}

bool canvas::_handle_cell_press(int x, int y) {
    // NOTE: negative row/col values underflow and don't pass the check below
    size_t col = (x - _field_shift.x) / _cfg.cell_width;
    size_t row = (y - _field_shift.y) / _cfg.cell_width;

    const core::dims& size = _grid.size();
    if (row >= size.rows || col >= size.cols) return false;

    auto ctrl = _ctrl.lock();
    if (_mouse_mode == mouse_modes::DRAW) {
        ctrl->engine_action1(row, col);
    } else if (_mouse_mode == mouse_modes::ERASE) {
        ctrl->engine_action2(row, col);
    }

    return true;
}

void canvas::_resize_grid() {
    auto ctrl = _ctrl.lock();
    // TODO: add checks for all locks()
    if (!ctrl) g_warning("failed to get controller to resize grid");
    ctrl->engine_resize({static_cast<size_t>(get_height() / _cfg.cell_width),  //
                         static_cast<size_t>(get_width() / _cfg.cell_width)});
}

}  // namespace automaton::app
