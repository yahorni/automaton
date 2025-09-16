#pragma once

#include <cstddef>
#include <format>
#include <string>

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

template<>
struct std::formatter<automaton::core::dims> : std::formatter<std::string> {
    auto format(automaton::core::dims dims, format_context& ctx) const {
        return formatter<string>::format(std::format("{}x{}", dims.rows, dims.cols), ctx);
    }
};
