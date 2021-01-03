#include "grid_area.hpp"

#include <iostream>  // debug

namespace automaton {

grid_area::grid_area()
    : _grid(_grid_y_cells, std::vector<char>(_grid_x_cells, false)) {
    // to catch mouse events
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_button_press_event().connect(
        sigc::mem_fun(*this, &grid_area::on_button_press));

    // to catch keyboard events
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;
    signal_key_press_event().connect(
        sigc::mem_fun(*this, &grid_area::on_key_press));

    // add draw function
    signal_draw().connect(sigc::mem_fun(*this, &grid_area::on_draw_cells));
}

grid_area::~grid_area() {}

bool grid_area::on_draw_cells(const Cairo::RefPtr<Cairo::Context>& cr) {
    draw_background(cr);
    draw_frame(cr);
    draw_grid_borders(cr);
    draw_grid_cells(cr);
    return false;
}

bool grid_area::on_button_press(GdkEventButton* ev) {
    if (ev->type == GDK_BUTTON_PRESS && (ev->button == 1 || ev->button == 3)) {
        size_t j = ev->x / _cell_width;
        size_t i = ev->y / _cell_width;

        if (i >= _grid_y_cells || j >= _grid_x_cells) return false;

        if (ev->button == 1)
            _grid[i][j] = true;
        else if (ev->button == 3)
            _grid[i][j] = false;

        queue_draw();
        return true;
    }

    return false;
}

bool grid_area::on_key_press(GdkEventKey* ev) {
    if (ev->keyval == GDK_KEY_s) {
        do_step();
        queue_draw();
        return true;
    } else if (ev->keyval == GDK_KEY_c) {
        clear();
        queue_draw();
        return true;
    }

    return false;
}

void grid_area::draw_background(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();
    cr->set_source_rgb(_bg_color.red, _bg_color.green, _bg_color.blue);
    cr->paint();
    cr->restore();
}

void grid_area::draw_frame(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();
    cr->set_source_rgb(_border_color.red, _border_color.green,
                       _border_color.blue);
    cr->set_line_width(_line_width);

    cr->rectangle(0, 0, get_grid_width(), get_grid_height());
    cr->stroke();

    cr->restore();
}

void grid_area::draw_grid_borders(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();
    cr->set_source_rgb(_border_color.red, _border_color.green,
                       _border_color.blue);
    cr->set_line_width(_line_width);

    double x = _cell_width, y = _cell_width;
    size_t width = get_grid_width(), height = get_grid_height();

    for (size_t j = 0; j < _grid_x_cells - 1; j++) {
        cr->move_to(x, 0);
        cr->line_to(x, height);
        x += _cell_width;
    }

    for (size_t i = 0; i < _grid_y_cells - 1; i++) {
        cr->move_to(0, y);
        cr->line_to(width, y);
        y += _cell_width;
    }

    cr->stroke();
    cr->restore();
}

void grid_area::draw_grid_cells(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();

    cr->set_source_rgb(_cell_color.red, _cell_color.green, _cell_color.blue);

    for (size_t i = 0; i < _grid_y_cells; i++) {
        for (size_t j = 0; j < _grid_x_cells; j++) {
            if (!_grid[i][j]) continue;

            auto xy = get_cell_xy(i, j);
            cr->rectangle(xy.first, xy.second, _cell_width, _cell_width);
            cr->fill();
        }
    }

    cr->restore();
}

double grid_area::get_grid_width() const { return _cell_width * _grid_x_cells; }

double grid_area::get_grid_height() const { return _cell_width * _grid_y_cells; }

std::pair<size_t, size_t> grid_area::get_cell_xy(size_t i, size_t j) const {
    return std::make_pair(_cell_width * j, _cell_width * i);
}

void grid_area::do_step() {
    // NOTE: start with grid.size - 2 to skip check for lowest level drop
    for (int i = _grid_y_cells - 2; i >= 0; i--) {
        for (size_t j = 0; j < _grid_x_cells; j++) {
            if (_grid[i][j]) {
                if (!_grid[i + 1][j]) {
                    _grid[i][j] = false;
                    _grid[i + 1][j] = true;
                } else if (j > 0 && !_grid[i + 1][j - 1]) {
                    _grid[i][j] = false;
                    _grid[i + 1][j - 1] = true;
                } else if (j < _grid_x_cells - 1 && !_grid[i + 1][j + 1]) {
                    _grid[i][j] = false;
                    _grid[i + 1][j + 1] = true;
                }
            }
        }
    }
}

void grid_area::clear() {
    for (size_t i = 0; i < _grid_y_cells; i++) {
        for (size_t j = 0; j < _grid_x_cells; j++) {
            if (_grid[i][j]) _grid[i][j] = false;
        }
    }
}

}  // namespace automaton
