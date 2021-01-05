#pragma once

#include <memory>
#include <set>
#include <vector>

#include "interfaces/base_cell.hpp"
#include "interfaces/base_grid.hpp"

namespace automaton {

class grid_2d;

class logic_2d {
   public:
    virtual void step(grid_2d& grid) = 0;
};

class grid_2d : public base_grid {
   public:
    grid_2d(size_t rows, size_t cols);
    ~grid_2d() override;

    void add(size_t row, size_t col) override;
    bool remove(size_t row, size_t col) override;
    bool has(size_t row, size_t col) override;
    void clear() override;

    void step() override;

    std::set<base_cell> get_drawable_cells() const override;

    void move(base_cell from, base_cell to);
    std::set<base_cell> get_data_copy() const;

   private:
    std::set<base_cell> _data;
    std::shared_ptr<logic_2d> _logic;
};

}  // namespace automaton
