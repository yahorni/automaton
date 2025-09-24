#pragma once

#include "automaton/core/dims.hpp"
#include "automaton/core/grid.hpp"
#include "gdk/gdk.h"

#include <gtkmm/drawingarea.h>

namespace automaton::app {

class controller;

struct palette {
    Gdk::RGBA surface_bg{"#fbfbfb"};
    Gdk::RGBA surface_fg{"#ff0000"};
    Gdk::RGBA border{"#000000"};
    Gdk::RGBA cell_state1{"#ffa500"};
    Gdk::RGBA cell_state2{"#008080"};
    Gdk::RGBA cell_state3{"#8b0000"};
};

struct canvas_config {
    core::dims initial_size;
    double cell_width;
    bool show_borders;
    bool show_status;
    bool adapt_to_window;
};

class canvas : public Gtk::DrawingArea {
public:
    explicit canvas(const core::grid& grid);
    void initialize(canvas_config cfg, std::weak_ptr<controller> ctrl);
    void on_resize();

private:
    using cairo_context = Cairo::RefPtr<Cairo::Context>;

    // events
    bool _on_key_press(GdkEventKey* ev);
    bool _on_mouse_press(GdkEventButton* ev);
    bool _on_mouse_release(GdkEventButton* ev);
    bool _on_mouse_motion(GdkEventMotion* ev);
    bool _on_mouse_scroll(GdkEventScroll* ev);
    bool _on_redraw_timeout();
    bool _on_draw(const cairo_context& cr);

    // drawing
    void _draw_background(const cairo_context& cr);
    void _draw_frame(const cairo_context& cr, const core::dims& size);
    void _draw_borders(const cairo_context& cr, const core::dims& size);
    void _draw_field(const cairo_context& cr, const core::dims& size, const core::grid_state& state);
    void _draw_status(const cairo_context& cr);

    // helpers
    void _draw_shifted_rectangle(const cairo_context& cr, double x, double y, double width, double height);
    void _draw_shifted_line(const cairo_context& cr, double x_from, double y_from, double x_to, double y_to);
    bool _handle_cell_press(int x, int y);
    void _resize_grid();

    const core::grid& _grid;

    canvas_config _cfg;
    std::weak_ptr<controller> _ctrl;

    const palette _palette;

    enum class mouse_modes { NONE, DRAW, ERASE, SHIFT } _mouse_mode;
    struct {
        double x, y;
    } _field_shift{0, 0}, _last_shift_start{0, 0};

    sigc::connection _redraw_connection;
};

}  // namespace automaton::app
