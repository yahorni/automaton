#pragma once

#include <memory>
#include <set>
#include <vector>

#include "interfaces/base_cell.hpp"
#include "interfaces/base_grid.hpp"

namespace automaton {

class grid_3d;

struct cell_3d : public base_cell {
    int level;

    cell_3d(size_t row, size_t col, int level = 0);
    bool operator<(const cell_3d& other) const;
};

class logic_3d {
   public:
    virtual void step(grid_3d& grid) = 0;
};

class grid_3d : public base_grid {
   public:
    typedef std::shared_ptr<logic_3d> logic_ptr;

    grid_3d(size_t rows, size_t cols);
    ~grid_3d() override;

    void add(size_t row, size_t col) override;
    bool remove(size_t row, size_t col) override;
    bool has(size_t row, size_t col) override;
    void clear() override;

    void step() override;

    std::set<base_cell> get_drawable_cells() const override;

    bool has(size_t row, size_t col, int level) const;
    void move(cell_3d from, cell_3d to);
    std::set<cell_3d> get_data_copy() const;

   private:
    std::set<cell_3d> _data;
    std::shared_ptr<logic_3d> _logic;
};

}  // namespace automaton
