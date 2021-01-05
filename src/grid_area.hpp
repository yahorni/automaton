#pragma once

#include <gtkmm/drawingarea.h>

#include <utility>
#include <vector>
#include <memory>

#include "interfaces/base_grid.hpp"
#include "falling_logic.hpp"
#include "utils/color.hpp"

namespace automaton {

class grid_area : public Gtk::DrawingArea {
   public:
    explicit grid_area(std::shared_ptr<base_grid> grid);
    virtual ~grid_area();

   private:
    bool on_draw_cells(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_button_press(GdkEventButton* ev);
    bool on_key_press(GdkEventKey* ev);

    void draw_background(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_grid_borders(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_grid_cells(const Cairo::RefPtr<Cairo::Context>& cr);

    double get_grid_width() const;
    double get_grid_height() const;
    std::pair<double, double> get_cell_xy(size_t row, size_t col) const;

    std::shared_ptr<base_grid> _grid;

    const color_t _bg_color{0.8, 0.8, 0.8};
    const color_t _border_color{0, 0, 0};
    const color_t _cell_color{0.756, 0.69, 0.569};

    const double _cell_width = 15;
    const double _line_width = 0.5;
};

}  // namespace automaton
