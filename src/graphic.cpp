#include "graphic.hpp"

#include <iostream>

namespace automaton {

CellArea::CellArea()
    : _grid(_grid_height, std::vector<char>(_grid_width, false)) {
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;

    signal_draw().connect(sigc::mem_fun(*this, &CellArea::on_draw_cells));
    signal_button_press_event().connect(sigc::mem_fun(*this, &CellArea::on_button_press));
    signal_key_press_event().connect(sigc::mem_fun(*this, &CellArea::on_key_press));
}

CellArea::~CellArea() {}

bool CellArea::on_draw_cells(const Cairo::RefPtr<Cairo::Context>& cr) {
    std::cout << "Drawed" << std::endl;

    draw_background(cr);
    draw_borders(cr);
    draw_grid(cr);

    return false;
}

bool CellArea::on_button_press(GdkEventButton* ev) {
    std::cout << "Mouse " << ev->type << " " << ev->x << " " << ev->y << std::endl;

    if (ev->type == GDK_BUTTON_PRESS) {
        size_t j = ev->x / (_cell_width + _line_width);
        size_t i = ev->y / (_cell_width + _line_width);

        if (i >= _grid.size() || j >= _grid[i].size()) return false;

        _grid[i][j] = !_grid[i][j];
        queue_draw();
        return true;
    }

    return false;
}

bool CellArea::on_key_press(GdkEventKey* ev) {
    std::cout << "Keyboard " << ev->keyval << " " << GDK_KEY_space << std::endl;

    if (ev->keyval == GDK_KEY_space) {
        do_step();

        queue_draw();
        return true;
    }

    return false;
}

void CellArea::draw_background(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();
    cr->set_source_rgb(0.8, 0.8, 0.8);
    cr->paint();
    cr->restore();
}

void CellArea::draw_borders(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();
    cr->set_source_rgb(0, 0, 0);
    cr->set_line_width(_line_width);

    size_t x = 0, y = 0;
    size_t width = get_grid_width(), height = get_grid_height();

    while (x < width) {
        x += _cell_width + _line_width;
        cr->move_to(x, 0);
        cr->line_to(x, height);
    }

    while (y < height) {
        y += _cell_width + _line_width;
        cr->move_to(0, y);
        cr->line_to(width, y);
    }

    cr->stroke();
    cr->restore();
}

void CellArea::draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();

    cr->set_source_rgb(0, 0, 0.9);

    for (size_t i = 0; i < _grid.size(); i++) {
        for (size_t j = 0; j < _grid[i].size(); j++) {
            if (!_grid[i][j]) continue;

            auto xy = get_cell_xy(i, j);
            cr->rectangle(xy.first, xy.second, _cell_width, _cell_width);
            cr->fill();
        }
    }

    cr->restore();
}

size_t CellArea::get_grid_width() const {
    return _cell_width * _grid_width + _line_width * (_grid_width - 1);
}

size_t CellArea::get_grid_height() const {
    return _cell_width * _grid_height + _line_width * (_grid_height - 1);
}

std::pair<size_t, size_t> CellArea::get_cell_xy(size_t i, size_t j) const {
    return std::make_pair((_cell_width + _line_width) * j,
                          (_cell_width + _line_width) * i);
}

void CellArea::do_step() {
    // NOTE: start with grid.size - 2 to skip check for lowest level drop
    for (int i = _grid.size() - 2; i >= 0; i--) {
        for (size_t j = 0; j < _grid[i].size(); j++) {
            if (_grid[i][j]) {
                if (!_grid[i + 1][j]) {
                    _grid[i][j] = false;
                    _grid[i + 1][j] = true;
                } else if (j > 0 && !_grid[i + 1][j - 1]) {
                    _grid[i][j] = false;
                    _grid[i + 1][j - 1] = true;
                } else if (j < _grid[i].size() - 1 && !_grid[i + 1][j + 1]) {
                    _grid[i][j] = false;
                    _grid[i + 1][j + 1] = true;
                }
            }
        }
    }
}

}  // namespace automaton
