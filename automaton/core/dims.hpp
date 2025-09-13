#pragma once

#include <cstddef>

namespace automaton::core {

struct dims {
    size_t rows;
    size_t cols;

    constexpr dims() = default;
    constexpr dims(int rows, int cols)
        : rows(rows),
          cols(cols) {}
    constexpr dims(size_t rows, size_t cols)
        : rows(rows),
          cols(cols) {}
};

}  // namespace automaton::core
