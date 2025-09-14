#pragma once

#include "automaton/core/dims.hpp"

#include <array>
#include <cstdint>

namespace automaton::core {

constexpr core::dims max_dims{600, 800};
using cell_state = std::uint8_t;
using grid_state = std::array<std::array<cell_state, max_dims.rows>, max_dims.cols>;

class grid {
public:
    cell_state get(size_t row, size_t col);
    void set(size_t row, size_t col, cell_state value);
    void clear();

    void resize(size_t rows, size_t cols);
    void reset(const grid_state& state);

    const core::dims& dims() const;
    const grid_state& state() const;

private:
    core::dims _dims{};
    grid_state _state{};
};

}  // namespace automaton::core
