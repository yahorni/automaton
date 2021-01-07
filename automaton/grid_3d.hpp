#pragma once

#include <automaton/base/cell.hpp>
#include <automaton/base/grid.hpp>
#include <memory>
#include <set>
#include <vector>

namespace automaton {

class grid_3d;

struct cell_3d : public base_cell {
    int level;

    cell_3d(size_t row, size_t col, int level = 0);
    bool operator<(const cell_3d& other) const;
};

class logic_3d : public base_logic {
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

    void set_logic(std::shared_ptr<base_logic> logic) override;
    void step() override;

    std::set<base_cell> get_drawable_cells() const override;

    void set_rows(size_t rows) override;
    void set_cols(size_t cols) override;

    // custom methods
    bool has(size_t row, size_t col, int level) const;
    void move(cell_3d from, cell_3d to);
    const std::set<cell_3d>& get_data() const;

   private:
    std::set<cell_3d> _data;
    std::shared_ptr<logic_3d> _logic;

    void update_sizes(size_t rows, size_t cols);
};

}  // namespace automaton
