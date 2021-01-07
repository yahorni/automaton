#pragma once

#include <gtkmm/drawingarea.h>

#include <memory>
#include <utility>
#include <vector>

#include "base/grid.hpp"
#include "falling_logic.hpp"
#include "utils/color.hpp"

namespace automaton {

class grid_area : public Gtk::DrawingArea {
   public:
    grid_area();
    virtual ~grid_area();

    void set_grid(std::shared_ptr<base_grid> grid);
    void set_grid_borders(bool borders);

   private:
    bool on_draw_cells(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_button_press(GdkEventButton* ev);
    bool on_button_release(GdkEventButton* ev);
    bool on_key_press(GdkEventKey* ev);
    bool on_motion(GdkEventMotion* ev);

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

    bool _grid_borders = false;

    const color_t _bg_color{0.8, 0.8, 0.8};
    const color_t _border_color{0, 0, 0};
    const color_t _cell_color{0.756, 0.69, 0.569};

    const double _cell_width = 15;
    const double _line_width = 0.5;
};

}  // namespace automaton
