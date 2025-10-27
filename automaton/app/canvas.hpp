#pragma once

#include "automaton/core/dims.hpp"
#include "automaton/core/grid.hpp"

#include <gtkmm/drawingarea.h>

#include <vector>

namespace automaton::app {

class controller;

// TODO: move colors to defaults.hpp
struct palette {
    Gdk::RGBA background{"#c0c0c0"};
    Gdk::RGBA font{"#ff0000"};
    Gdk::RGBA border{"#000000"};
    std::vector<Gdk::RGBA> cell_states = {
        // clang-format off
        Gdk::RGBA{"#fbfbfb"},
        Gdk::RGBA{"#ffa500"},
        Gdk::RGBA{"#008080"},
        Gdk::RGBA{"#8b0000"}
        // clang-format on
    };
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
    void initialize(canvas_config cfg, std::shared_ptr<controller> ctrl);
    void on_resize();

private:
    using cairo_context = Cairo::RefPtr<Cairo::Context>;

    enum class mouse_modes { NONE, DRAW, ERASE } _mouse_mode;

    struct vec2 {
        double x = 0;
        double y = 0;
    };

    // events
    bool _on_key_press(GdkEventKey* ev);
    bool _on_mouse_press(GdkEventButton* ev);
    bool _on_mouse_release(GdkEventButton* ev);
    bool _on_mouse_motion(GdkEventMotion* ev);
    bool _on_mouse_scroll(GdkEventScroll* ev);
    bool _on_redraw_timeout();
    bool _on_draw(const cairo_context& cr);

    // drawing
    void _draw_background(const cairo_context& cr) const;
    void _draw_frame(const cairo_context& cr, const vec2& real_size, const vec2& field_start) const;
    void _draw_field(const cairo_context& cr, const core::dims& size, const vec2& field_start) const;
    void _draw_borders(const cairo_context& cr, const core::dims& size, const vec2& real_size,
                       const vec2& field_start) const;
    void _draw_status(const cairo_context& cr) const;

    // helpers

    vec2 _calculate_real_size() const;
    vec2 _calculate_field_start(const vec2& real_size) const;

    bool _handle_cell_press(int x, int y);
    void _resize_grid();

    const core::grid& _grid;

    canvas_config _cfg;
    std::shared_ptr<controller> _ctrl;

    const palette _palette;

    vec2 _field_shift;
    std::optional<vec2> _shift_start;

    sigc::connection _redraw_connection;
};

}  // namespace automaton::app
