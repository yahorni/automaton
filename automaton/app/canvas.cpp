#include "automaton/app/canvas.hpp"

#include "automaton/app/controller.hpp"
#include "automaton/core/grid.hpp"
#include "gdkmm/rgba.h"

#include <cairomm/fontface.h>
#include <gdk/gdk.h>
#include <gdkmm/general.h>
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
    // catch mouse leave/enter events
    add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
    signal_enter_notify_event().connect(sigc::mem_fun(*this, &canvas::_on_mouse_enter));
    signal_leave_notify_event().connect(sigc::mem_fun(*this, &canvas::_on_mouse_leave));

    // catch keyboard events
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;
    signal_key_press_event().connect(sigc::mem_fun(*this, &canvas::_on_key_press));

    // add draw function
    signal_draw().connect(sigc::mem_fun(*this, &canvas::_on_draw));
}

void canvas::initialize(canvas_config cfg, std::weak_ptr<controller> ctrl) {
    _cfg = cfg;
    _ctrl = ctrl;
}

bool canvas::_on_draw(const cairo_context& cr) {
    const core::dims& dims = _grid.dims();
    const core::grid_state& state = _grid.state();

    _draw_background(cr);
    _draw_frame(cr, dims);
    if (_cfg.use_borders) _draw_grid_borders(cr, dims);
    _draw_cells(cr, dims, state);
    _draw_information(cr);

    return false;
}

bool canvas::_on_key_press(GdkEventKey* ev) {
    g_debug("canvas::on_key_press(key='%s')", ev->string);

    if (ev->keyval == GDK_KEY_space) {
        _ctrl.lock()->toggle_animation();
    } else if (ev->keyval == GDK_KEY_s) {
        _ctrl.lock()->step();
    } else if (ev->keyval == GDK_KEY_c) {
        _ctrl.lock()->clear_grid();
    } else if (ev->keyval == GDK_KEY_r) {
        _ctrl.lock()->restart_steps();
    } else {
        return false;
    }

    queue_draw();
    return true;
}

bool canvas::_on_mouse_press(GdkEventButton* ev) {
    g_debug("canvas::on_mouse_press()");

    if (ev->type != GDK_BUTTON_PRESS) return false;

    if (ev->button == 1) {  // left click
        _is_drawing = true;
        if (!_handle_cell_press(ev->x, ev->y)) return false;

    } else if (ev->button == 3) {  // right click
        _is_erasing = true;
        if (!_handle_cell_press(ev->x, ev->y)) return false;

    } else if (ev->button == 2) {  // middle click
        auto ctrl = _ctrl.lock();
        if (!ctrl) return true;
        ctrl->toggle_animation();

    } else {
        return false;
    }

    queue_draw();
    return true;
}

bool canvas::_on_mouse_motion(GdkEventMotion* ev) {
    /* g_debug("canvas::on_mouse_motion()"); */

    if (_is_drawing || _is_erasing) {
        if (!_handle_cell_press(ev->x, ev->y)) return false;
        queue_draw();
        return true;
    }

    return false;
}

bool canvas::_on_mouse_release(GdkEventButton* ev) {
    g_debug("canvas::on_mouse_release()");

    if (ev->button == 1 && _is_drawing) {
        _is_drawing = false;
        return true;
    } else if (ev->button == 3 && _is_erasing) {
        _is_erasing = false;
        return true;
    }

    return false;
}

bool canvas::_on_mouse_enter(GdkEventCrossing* ev) {
    g_debug("canvas::on_mouse_enter()");
    (void)ev;
    return false;
}

bool canvas::_on_mouse_leave(GdkEventCrossing* ev) {
    g_debug("canvas::on_mouse_leave()");
    (void)ev;
    return false;
}

void canvas::_draw_background(const cairo_context& cr) {
    cr->save();
    Gdk::Cairo::set_source_rgba(cr, _palette.surface_bg);
    cr->paint();
    cr->restore();
}

void canvas::_draw_frame(const cairo_context& cr, const core::dims& dims) {
    cr->save();
    Gdk::Cairo::set_source_rgba(cr, _palette.border);
    cr->set_line_width(_cfg.line_width);

    cr->rectangle(0, 0, _cfg.cell_width * dims.cols, _cfg.cell_width * dims.rows);
    cr->stroke();

    cr->restore();
}

void canvas::_draw_grid_borders(const cairo_context& cr, const core::dims& dims) {
    cr->save();
    Gdk::Cairo::set_source_rgba(cr, _palette.border);
    cr->set_line_width(_cfg.line_width);

    size_t width = _cfg.cell_width * dims.cols, height = _cfg.cell_width * dims.rows;
    double x = _cfg.cell_width, y = _cfg.cell_width;

    for (size_t col = 0; col < dims.cols - 1; col++) {
        cr->move_to(x, 0);
        cr->line_to(x, height);
        x += _cfg.cell_width;
    }

    for (size_t row = 0; row < dims.rows - 1; row++) {
        cr->move_to(0, y);
        cr->line_to(width, y);
        y += _cfg.cell_width;
    }

    cr->stroke();
    cr->restore();
}

// cell {0,0} is at the top-left corner of the screen
void canvas::_draw_cells(const cairo_context& cr, const core::dims& dims, const core::grid_state& state) {
    cr->save();

    const auto draw_cells_with_state = [&](std::uint8_t cell_state, const Gdk::RGBA& color) {
        Gdk::Cairo::set_source_rgba(cr, color);
        for (size_t row = 0; row < dims.rows; ++row) {
            for (size_t col = 0; col < dims.cols; ++col) {
                if (state[row][col] == cell_state) {
                    cr->rectangle(_cfg.cell_width * col,  //
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

    cr->restore();
}

void canvas::_draw_information(const cairo_context& cr) {
    cr->save();
    cr->move_to(0, _cfg.font_size);

    Gdk::Cairo::set_source_rgba(cr, _palette.surface_fg);
    cr->set_font_size(_cfg.font_size);
    cr->select_font_face("", Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_NORMAL);
    cr->show_text(_ctrl.lock()->get_status());

    cr->restore();
}

void canvas::handle_timeout() {
    g_debug("canvas::handle_timeout()");

    if (_is_drawing || _is_erasing) {
        int x, y;
        get_pointer(x, y);
        _handle_cell_press(x, y);
    }

    queue_draw();
}

bool canvas::_handle_cell_press(int x, int y) {
    size_t col = x / _cfg.cell_width;
    size_t row = y / _cfg.cell_width;

    const core::dims& dims = _grid.dims();
    if (row >= dims.rows || col >= dims.cols) return false;

    auto ctrl = _ctrl.lock();
    if (_is_drawing) {
        ctrl->add_cell(row, col);
    } else if (_is_erasing) {
        ctrl->remove_cell(row, col);
    }

    return true;
}

void canvas::redraw() { queue_draw(); }

core::dims canvas::calculate_dims() const {
    return {static_cast<size_t>(get_height() / _cfg.cell_width),  //
            static_cast<size_t>(get_width() / _cfg.cell_width)};
}

}  // namespace automaton::app
