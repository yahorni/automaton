#include "grid_2d.hpp"

#include "falling_logic.hpp"

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

std::set<base_cell> grid_2d::get_data_copy() const { return _data; }

}  // namespace automaton
