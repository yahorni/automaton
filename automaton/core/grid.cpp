#include "automaton/core/grid.hpp"

#include <algorithm>

namespace automaton::core {

void grid::add(size_t row, size_t col) { set(row, col, 1); }

void grid::remove(size_t row, size_t col) { set(row, col, 0); }

void grid::set(size_t row, size_t col, std::uint8_t value) { _state[row][col] = value; }

const grid_state& grid::state() const { return _state; }

void grid::clear() { _state = {}; }

void grid::resize(size_t rows, size_t cols) {
    _dims.rows = std::min(rows, max_dims.rows);
    _dims.cols = std::min(cols, max_dims.cols);
}

void grid::reset(const grid_state& state) { _state = state; }

const core::dims& grid::dims() const { return _dims; }

}  // namespace automaton::core
