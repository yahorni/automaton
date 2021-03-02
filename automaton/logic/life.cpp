#include <automaton/logic/life.hpp>
#include <vector>

namespace automaton {
namespace logic {

void life_2d::step(base_grid& grid) {
    size_t rows = grid.get_rows(), cols = grid.get_cols();
    std::set<base_cell> new_state;

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            size_t prev_row = (row == 0 ? rows - 1 : row - 1);
            size_t next_row = (row == rows - 1 ? 0 : row + 1);

            size_t prev_col = (col == 0 ? cols - 1 : col - 1);
            size_t next_col = (col == cols - 1 ? 0 : col + 1);

            uint8_t neighbours = 0;
            if (grid.has(prev_row, prev_col)) ++neighbours;
            if (grid.has(prev_row, col)) ++neighbours;
            if (grid.has(prev_row, next_col)) ++neighbours;
            if (grid.has(row, prev_col)) ++neighbours;
            if (grid.has(row, next_col)) ++neighbours;
            if (grid.has(next_row, prev_col)) ++neighbours;
            if (grid.has(next_row, col)) ++neighbours;
            if (grid.has(next_row, next_col)) ++neighbours;

            if (!grid.has(row, col) && neighbours == 3) new_state.emplace(row, col);

            if (grid.has(row, col) && (neighbours == 2 || neighbours == 3)) new_state.emplace(row, col);
        }
    }

    grid.clear();

    for (const auto& cell : new_state)
        grid.add(cell.row, cell.col);
}

void life_2d::clear() {}

} // namespace logic
} // namespace automaton
