#include "automaton/core/grid.hpp"

#include <algorithm>

namespace automaton::core {

cell_state grid::get(size_t row, size_t col) { return _state[row][col]; }

void grid::set(size_t row, size_t col, cell_state value) { _state[row][col] = value; }

void grid::clear() { _state = {}; }

void grid::resize(size_t rows, size_t cols) {
    _dims.rows = std::min(rows, max_dims.rows);
    _dims.cols = std::min(cols, max_dims.cols);
}

void grid::reset(const grid_state& state) { _state = state; }

const core::dims& grid::dims() const { return _dims; }
const grid_state& grid::state() const { return _state; }

}  // namespace automaton::core
