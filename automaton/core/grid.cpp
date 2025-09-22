#include "automaton/core/grid.hpp"

#include <algorithm>

namespace automaton::core {

cell_state grid::get(size_t row, size_t col) { return _state[row][col]; }

void grid::set(size_t row, size_t col, cell_state value) { _state[row][col] = value; }

void grid::clear() { _state = {}; }

void grid::resize(const core::dims& size) {
    _dims.rows = std::min(size.rows, static_cast<size_t>(defaults::grid_max_rows));
    _dims.cols = std::min(size.cols, static_cast<size_t>(defaults::grid_max_cols));
}

void grid::reset(const grid_state& state) { _state = state; }

const core::dims& grid::dims() const { return _dims; }
const grid_state& grid::state() const { return _state; }

}  // namespace automaton::core
