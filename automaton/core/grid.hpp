#pragma once

#include "automaton/core/dims.hpp"

#include <array>
#include <cstdint>

namespace automaton::core {

constexpr core::dims max_dims{600, 800};
using grid_state = std::array<std::array<std::uint8_t, max_dims.rows>, max_dims.cols>;

class grid {
public:
    void add(size_t row, size_t col);
    void remove(size_t row, size_t col);
    void set(size_t row, size_t col, std::uint8_t value);
    void clear();
    void resize(size_t rows, size_t cols);
    void reset(const grid_state& state);

    const grid_state& state() const;
    const core::dims& dims() const;

private:
    core::dims _dims{};
    grid_state _state{};
};

}  // namespace automaton::core
