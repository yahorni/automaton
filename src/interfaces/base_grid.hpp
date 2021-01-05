#pragma once

#include <set>

#include "base_cell.hpp"

namespace automaton {

class base_grid {
   public:
    base_grid(size_t rows, size_t cols) : _rows(rows), _cols(cols) {}
    virtual ~base_grid() {}

    virtual void add(size_t row, size_t col) = 0;
    virtual bool remove(size_t row, size_t col) = 0;
    virtual bool has(size_t row, size_t col) = 0;
    virtual void clear() = 0;

    virtual void step() = 0;

    virtual std::set<base_cell> get_drawable_cells() const = 0;

    size_t get_rows() const { return _rows; }
    size_t get_cols() const { return _cols; }

   protected:
    size_t _rows, _cols;
};

}  // namespace automaton
