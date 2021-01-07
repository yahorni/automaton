#include <automaton/grid_2d.hpp>
#include <automaton/logic/falling.hpp>

namespace automaton {

grid_2d::grid_2d(size_t rows, size_t cols)
    : base_grid(rows, cols),
      _logic(std::static_pointer_cast<logic_2d>(
          std::make_shared<falling_logic_2d>())) {}

grid_2d::~grid_2d() {}

void grid_2d::add(size_t row, size_t col) { _data.emplace(row, col); }

bool grid_2d::has(size_t row, size_t col) {
    return _data.count(base_cell{row, col}) == 1;
}

bool grid_2d::remove(size_t row, size_t col) {
    for (auto it = _data.begin(); it != _data.end(); it++) {
        if (it->row == row && it->col == col) {
            _data.erase(it);
            return true;
        }
    }

    return false;
}

void grid_2d::clear() { _data.clear(); }

void grid_2d::step() { _logic->step(*this); }

std::set<base_cell> grid_2d::get_drawable_cells() const {
    return {_data.begin(), _data.end()};
}

void grid_2d::move(base_cell from, base_cell to) {
    _data.erase(_data.find(from));
    _data.insert(to);
}

const std::set<base_cell>& grid_2d::get_data() const { return _data; }

void grid_2d::set_rows(size_t rows) {
    base_grid::set_rows(rows);
    update_sizes(rows, _cols);
}

void grid_2d::set_cols(size_t cols) {
    base_grid::set_cols(cols);
    update_sizes(_rows, cols);
}

void grid_2d::update_sizes(size_t rows, size_t cols) {
    for (auto it = _data.begin(); it != _data.end();) {
        if (it->row > rows - 1 || it->col > cols - 1)
            it = _data.erase(it);
        else
            it++;
    }
}

}  // namespace automaton
