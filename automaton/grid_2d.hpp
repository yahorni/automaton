#pragma once

#include <automaton/base/cell.hpp>
#include <automaton/base/grid.hpp>
#include <memory>
#include <set>
#include <vector>

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

    void set_rows(size_t rows) override;
    void set_cols(size_t cols) override;

    // custom methods
    void move(base_cell from, base_cell to);
    const std::set<base_cell>& get_data() const;

   private:
    std::set<base_cell> _data;
    std::shared_ptr<logic_2d> _logic;

    void update_sizes(size_t rows, size_t cols);
};

}  // namespace automaton
