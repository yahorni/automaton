#pragma once

#include <automaton/base/cell.hpp>
#include <automaton/base/grid.hpp>
#include <memory>
#include <set>
#include <vector>

namespace automaton {

struct cell_3d : public base_cell {
    int level;

    cell_3d(size_t row, size_t col, int level = 0);
    bool operator<(const cell_3d& other) const;
};

class grid_3d : public base_grid {
   public:
    typedef std::shared_ptr<base_logic> logic_ptr;

    grid_3d(size_t rows, size_t cols);
    ~grid_3d() override;

    void add(size_t row, size_t col) override;
    bool remove(size_t row, size_t col) override;
    bool has(size_t row, size_t col) override;
    void clear() override;

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

    void update_sizes(size_t rows, size_t cols);
};

}  // namespace automaton
