#pragma once

#include "automaton/core/dims.hpp"
#include "automaton/core/grid.hpp"

#include <gtkmm/drawingarea.h>

namespace automaton::app {

class controller;

struct palette {
    Gdk::RGBA surface_bg{"#fbfbfb"};
    Gdk::RGBA surface_fg{"#ff0000"};
    Gdk::RGBA border{"#000000"};
    Gdk::RGBA cell_state1{"#ffa500"};
    Gdk::RGBA cell_state2{"#008080"};
};

struct canvas_config {
    double cell_width;
    bool use_borders;
    double line_width = 0.5;
    int font_size = 14;
};

class canvas : public Gtk::DrawingArea {
public:
    using cairo_context = Cairo::RefPtr<Cairo::Context>;

    explicit canvas(const core::grid& grid);
    void initialize(canvas_config cfg, std::weak_ptr<controller> ctrl);
    void handle_timeout();
    void redraw();

    core::dims calculate_dims() const;

private:
    using const_grid_ptr = std::shared_ptr<const core::grid>;

    // events
    bool _on_draw(const cairo_context& cr);
    bool _on_key_press(GdkEventKey* ev);
    bool _on_mouse_press(GdkEventButton* ev);
    bool _on_mouse_release(GdkEventButton* ev);
    bool _on_mouse_motion(GdkEventMotion* ev);
    bool _on_mouse_enter(GdkEventCrossing* ev);
    bool _on_mouse_leave(GdkEventCrossing* ev);

    // drawing
    void _draw_background(const cairo_context& cr);
    void _draw_frame(const cairo_context& cr, const core::dims& dims);
    void _draw_grid_borders(const cairo_context& cr, const core::dims& dims);
    void _draw_cells(const cairo_context& cr, const core::dims& dims, const core::grid_state& state);
    void _draw_information(const cairo_context& cr);

    bool _handle_cell_press(int x, int y);

    const core::grid& _grid;

    canvas_config _cfg;
    std::weak_ptr<controller> _ctrl;

    const palette _palette;
    bool _is_drawing = false;
    bool _is_erasing = false;
};

}  // namespace automaton::app
