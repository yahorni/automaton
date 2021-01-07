#pragma once

#include <gtkmm/drawingarea.h>

#include <automaton/base/grid.hpp>
#include <memory>
#include <utility>
#include <vector>

namespace automaton {

class grid_area : public Gtk::DrawingArea {
   public:
    grid_area();
    virtual ~grid_area();

    void set_grid(std::shared_ptr<base_grid> grid);
    void set_grid_borders(bool borders);
    void set_step_delay(size_t delay);

    double get_cell_width() const;

   private:
    bool on_draw_cells(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_button_press(GdkEventButton* ev);
    bool on_button_release(GdkEventButton* ev);
    bool on_key_press(GdkEventKey* ev);
    bool on_motion(GdkEventMotion* ev);
    bool on_timeout();

    void toggle_ongoing();

    void draw_background(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_grid_borders(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_grid_cells(const Cairo::RefPtr<Cairo::Context>& cr);

    double get_grid_width() const;
    double get_grid_height() const;
    std::pair<double, double> get_cell_xy(size_t row, size_t col) const;

    std::shared_ptr<base_grid> _grid;

    bool _is_drawing = false;
    bool _is_clearing = false;

    bool _is_ongoing = false;
    size_t _delay = 100;
    sigc::connection _ongoing_connection;

    bool _grid_borders = false;

    const Gdk::Color _bg_color{"#fbfbfb"};
    const Gdk::Color _border_color{"#000000"};
    const Gdk::Color _cell_color{"#008080"};

    const double _cell_width = 15;
    const double _line_width = 0.5;
};

}  // namespace automaton
