#pragma once

#include "automaton/core/defaults.hpp"
#include "automaton/core/dims.hpp"

#include <array>
#include <cstdint>

namespace automaton::core {

using cell_state = std::uint8_t;
using grid_state = std::array<std::array<cell_state, defaults::grid::max_rows>, defaults::grid::max_cols>;

class grid {
public:
    cell_state get(size_t row, size_t col);
    void set(size_t row, size_t col, cell_state value);
    void clear();

    void resize(const core::dims& size);
    void reset(const grid_state& state);

    const core::dims& size() const;
    const grid_state& state() const;

private:
    core::dims _size{defaults::grid::initial_rows, defaults::grid::initial_cols};
    grid_state _state{};
};

}  // namespace automaton::core
