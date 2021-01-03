#pragma once

#include <vector>
#include <utility>
#include <gtkmm/drawingarea.h>

namespace automaton {

struct color_t {
    double red;
    double green;
    double blue;
};

class grid_area : public Gtk::DrawingArea {
public:
    grid_area();
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

    void do_step();
    void clear();

    color_t _bg_color{0.8, 0.8, 0.8};
    color_t _border_color{0, 0, 0};
    color_t _cell_color{0.756, 0.69, 0.569};

    std::pair<size_t, size_t> get_cell_xy(size_t i, size_t j) const;

    double _cell_width = 15;
    double _line_width = 0.5;

    size_t _grid_x_cells = 50;
    size_t _grid_y_cells = 40;
    std::vector<std::vector<char>> _grid;
};

} // namespace automaton
