#pragma once

#include <vector>
#include <utility>
#include <gtkmm/drawingarea.h>

namespace automaton {

class CellArea : public Gtk::DrawingArea {
public:
    CellArea();
    virtual ~CellArea();

private:
    bool on_draw_cells(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_button_press(GdkEventButton* ev);
    bool on_key_press(GdkEventKey* ev);

    void draw_background(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_borders(const Cairo::RefPtr<Cairo::Context>& cr);
    void draw_grid(const Cairo::RefPtr<Cairo::Context>& cr);

    size_t get_grid_width() const;
    size_t get_grid_height() const;

    void do_step();

    std::pair<size_t, size_t> get_cell_xy(size_t i, size_t j) const;

    size_t _cell_width = 19;
    size_t _line_width = 1;

    size_t _grid_width = 40;
    size_t _grid_height = 30;
    std::vector<std::vector<char>> _grid;
};

} // namespace automaton
