#pragma once

#include "automaton/core/defaults.hpp"
#include "automaton/core/dims.hpp"

#include <array>
#include <cstdint>

namespace automaton::core {

using cell_state = uint8_t;
using grid_state = std::array<std::array<cell_state, defaults::grid::max_rows>, defaults::grid::max_cols>;

class grid {
public:
    cell_state get(size_t row, size_t col);
    void set(size_t row, size_t col, cell_state value);
    void clear();

    void resize(const core::dims& size);
    void reset(const grid_state& state);

    const core::dims& size() const { return _size; }
    size_t rows() const { return _size.rows; };
    size_t cols() const { return _size.cols; };

    cell_state operator[](std::size_t row, std::size_t col) const { return _state[row][col]; }

private:
    core::dims _size{0, 0};
    grid_state _state{};
};

}  // namespace automaton::core
