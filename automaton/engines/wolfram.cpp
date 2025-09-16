#include "automaton/engines/wolfram.hpp"

#include <format>

namespace automaton::engines {

wolfram::wolfram(core::grid& grid, core::surface_type surface, std::uint8_t code)
    : engine(grid, core::engine_type::WOLFRAM, surface),
      _code(code) {}

std::string wolfram::description() const {
    return std::format("wolfram[surface={},rule={},size={},step={}]",  //
                       options::surface::to_string(_surface_type), _code, _grid.dims(), _step);
}

bool wolfram::step() {
    constexpr int active_state = 1;
    constexpr int inactive_state = 2;

    const core::dims& dims = _grid.dims();
    const core::grid_state& state = _grid.state();

    // when starting: fast forward to the first row with active cells
    if (_current_row == 0) {
        bool found = false;
        for (size_t row = 0; row < dims.rows; ++row) {
            size_t col;
            for (col = 0; col < dims.cols; ++col) {
                if (state[row][col] == active_state) {
                    _current_row = row;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            _current_row = 0;
            return false;
        }
    }

    // set current row as inactive
    for (size_t col = 0; col < dims.cols; col++) {
        if (state[_current_row][col] == 1) _grid.set(_current_row, col, inactive_state);
    }

    // reached the bottom, no further steps available
    if (_current_row + 1 == dims.rows) {
        _current_row = 0;
        return false;
    }

    // calculate new state of the next row
    for (size_t col = 0; col < dims.cols; col++) {
        std::uint8_t combination = 0;

        if (_surface_type == core::surface_type::CYLINDER) {
            size_t prev_col = (col == 0 ? dims.cols - 1 : col - 1);
            size_t next_col = (col == dims.cols - 1 ? 0 : col + 1);

            if (state[_current_row][prev_col]) combination |= 0b001;
            if (state[_current_row][col]) combination |= 0b010;
            if (state[_current_row][next_col]) combination |= 0b100;
        } else {  // core::surface_type::PLAIN
            if (col != 0 && state[_current_row][col - 1]) combination |= 0b001;
            if (state[_current_row][col]) combination |= 0b010;
            if (col != dims.cols - 1 && state[_current_row][col + 1]) combination |= 0b100;
        }

        if ((_code >> combination) & 0b1) _grid.set(_current_row + 1, col, active_state);
    }

    _current_row++;
    _step++;
    return true;
}

void wolfram::restart() {
    engine::restart();
    _current_row = 0;
}

void wolfram::clear() {
    engine::clear();
    _current_row = 0;
}

}  // namespace automaton::engines
