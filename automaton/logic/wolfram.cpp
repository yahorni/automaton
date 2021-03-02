#include <automaton/grid/_1d.hpp>
#include <automaton/logic/wolfram.hpp>

namespace automaton {
namespace logic {

wolfram::wolfram(int8_t code) : _code(code) {}

void wolfram::step(base_grid& _grid) {
    auto& grid = static_cast<grid_1d&>(_grid);
    size_t rows = grid.get_rows(), cols = grid.get_cols();

    if (_current_step + 1 == rows) return;

    // when starting: fast forward to first row with cells (ignoring last row)
    if (_current_step == 0 && grid.get_data().size() > 0 && grid.get_data().begin()->row != rows - 1)
        _current_step = grid.get_data().begin()->row;

    size_t row = _current_step;

    for (size_t col = 0; col < cols; col++) {
        size_t prev_col = (col == 0 ? cols - 1 : col - 1);
        size_t next_col = (col == cols - 1 ? 0 : col + 1);

        int8_t combination = 0;

        if (grid.has(row, prev_col)) combination |= 0b001;
        if (grid.has(row, col)) combination |= 0b010;
        if (grid.has(row, next_col)) combination |= 0b100;

        if ((_code >> combination) & 0b1) grid.add(row + 1, col);
    }

    _current_step++;
}

void wolfram::clear() { _current_step = 0; }

} // namespace logic
} // namespace automaton
