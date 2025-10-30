#include "automaton/core/grid.hpp"

#include "automaton/core/defaults.hpp"

namespace automaton::core {

cell_state grid::get(size_t row, size_t col) { return _state[row][col]; }

void grid::set(size_t row, size_t col, cell_state value) { _state[row][col] = value; }

void grid::clear() { _state = {}; }

void grid::resize(const core::dims& size) {
    if (size.rows < defaults::grid::min_rows || size.rows > defaults::grid::max_rows ||
        size.cols < defaults::grid::min_cols || size.cols > defaults::grid::max_cols)
        return;

    if (size.rows < _size.rows) {
        for (size_t i = size.rows; i < _size.rows; ++i) {
            _state[i].fill(0);
        }
    }
    if (size.cols < _size.cols) {
        for (size_t i = 0; i < size.rows; ++i) {
            for (size_t j = size.cols; j < _size.cols; ++j) {
                _state[i][j] = 0;
            }
        }
    }

    _size = size;
}

void grid::reset(const grid_state& state) { _state = state; }

}  // namespace automaton::core
