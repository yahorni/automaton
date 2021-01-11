#pragma once

#include <stddef.h>

namespace automaton {

struct base_cell {
    size_t row;
    size_t col;

    base_cell(size_t row, size_t col) : row(row), col(col) {}
    virtual bool operator<(const base_cell& other) const {
        if (row == other.row) return col < other.col;
        // return backwards to put upper row in std::set first
        return row > other.row;
    }
};

}  // namespace automaton
