#include "automaton/engines/wolfram.hpp"

#include <format>

namespace automaton::engines {

wolfram::wolfram(core::grid& grid, std::uint8_t code, core::surface_type surface)
    : engine(core::engine_type::WOLFRAM, surface),
      _grid(grid),
      _code(code),
      _surface(surface) {}

bool wolfram::step() {
    static constexpr int active_state = 1;
    static constexpr int inactive_state = 2;

    const core::dims& dims = _grid.dims();
    const core::grid_state& state = _grid.state();

    // when starting: fast forward to the first row with cells (ignoring last row)
    if (_current_row == 0) {
        size_t fwd_row = 0;
        for (; fwd_row < dims.rows; ++fwd_row) {
            size_t col;
            for (col = 0; col < dims.cols; ++col) {
                if (state[fwd_row][col] == active_state) break;
            }
            if (col != dims.cols) break;
        }
        _current_row = fwd_row;
    }

    // set current row as inactive
    for (size_t col = 0; col < dims.cols; col++) {
        if (state[_current_row][col] == 1) _grid.set(_current_row, col, inactive_state);
    }

    // reached the bottom, no further steps available
    if (_current_row + 1 == dims.rows) {
        return false;
    }

    // calculate new state of the next row
    for (size_t col = 0; col < dims.cols; col++) {
        std::uint8_t combination = 0;

        if (_surface == core::surface_type::CYLINDER) {
            size_t prev_col = (col == 0 ? dims.cols - 1 : col - 1);
            size_t next_col = (col == dims.cols - 1 ? 0 : col + 1);

            if (state[_current_row][prev_col]) combination |= 0b001;
            if (state[_current_row][col]) combination |= 0b010;
            if (state[_current_row][next_col]) combination |= 0b100;
        } else {
            if (col != 0 && state[_current_row][col - 1]) combination |= 0b001;
            if (state[_current_row][col]) combination |= 0b010;
            if (col != dims.cols - 1 && state[_current_row][col + 1]) combination |= 0b100;
        }

        if ((_code >> combination) & 1) _grid.set(_current_row + 1, col, active_state);
    }

    _current_row++;
    return true;
}

void wolfram::reset() { _current_row = 0; }

const std::string& wolfram::name() const {
    static std::string name =
        std::format("wolfram[surface={},rule={}]", options::surface::to_string(_surface_type), _code);
    return name;
}

}  // namespace automaton::engines
