#include "automaton/engines/wolfram.hpp"

#include <format>

namespace automaton::engines {

wolfram::wolfram(core::grid& grid, core::surface_type surface, std::uint8_t code)
    : engine(grid, core::engine_type::WOLFRAM, surface),
      _code(code) {}

std::string wolfram::description() const {
    return std::format("wolfram[surface={},rule={},size={},row={},step={}]",  //
                       options::surface::to_string(_surface_type), _code, _grid.size(),
                       (_current_row == no_row_selected ? -1 : static_cast<int>(_current_row)), _step);
}

enum states { EMPTY, EMPTY_CURRENT, FILLED, FILLED_CURRENT };

void wolfram::highlight_row() {
    const core::dims& size = _grid.size();
    const core::grid_state& state = _grid.state();

    for (size_t col = 0; col < size.cols; col++) {
        if (state[_current_row][col] == states::FILLED)
            _grid.set(_current_row, col, states::FILLED_CURRENT);
        else if (state[_current_row][col] == states::EMPTY)
            _grid.set(_current_row, col, states::EMPTY_CURRENT);
    }
}

void wolfram::dehighlight_row() {
    const core::dims& size = _grid.size();
    const core::grid_state& state = _grid.state();

    for (size_t col = 0; col < size.cols; col++) {
        if (state[_current_row][col] == states::FILLED_CURRENT)
            _grid.set(_current_row, col, states::FILLED);
        else if (state[_current_row][col] == states::EMPTY_CURRENT)
            _grid.set(_current_row, col, states::EMPTY);
    }
}

bool wolfram::step() {
    const core::dims& size = _grid.size();
    const core::grid_state& state = _grid.state();

    // when starting: fast forward to the first row with active cells
    if (_current_row == no_row_selected) {
        for (size_t row = 0; row < size.rows; ++row) {
            for (size_t col = 0; col < size.cols; ++col) {
                if (state[row][col] == states::FILLED) {
                    _current_row = row;
                    highlight_row();
                    return true;
                }
            }
        }
    }

    dehighlight_row();

    // reached the bottom, no further steps available
    if (_current_row + 1 == size.rows) {
        _current_row = no_row_selected;
        return false;
    }

    // calculate new state of the next row
    for (size_t col = 0; col < size.cols; col++) {
        std::uint8_t combination = 0;

        if (_surface_type == core::surface_type::CYLINDER) {
            size_t prev_col = (col == 0 ? size.cols - 1 : col - 1);
            size_t next_col = (col == size.cols - 1 ? 0 : col + 1);

            if (state[_current_row][prev_col] == FILLED) combination |= 0b001;
            if (state[_current_row][col] == FILLED) combination |= 0b010;
            if (state[_current_row][next_col] == FILLED) combination |= 0b100;
        } else {  // core::surface_type::PLAIN
            if (col != 0 && state[_current_row][col - 1] == FILLED) combination |= 0b001;
            if (state[_current_row][col] == FILLED) combination |= 0b010;
            if (col != size.cols - 1 && state[_current_row][col + 1] == FILLED) combination |= 0b100;
        }

        if ((_code >> combination) & 0b1) _grid.set(_current_row + 1, col, states::FILLED);
    }

    _current_row++;
    highlight_row();

    _step++;
    return true;
}

void wolfram::restart() {
    engine::restart();
    dehighlight_row();
    _current_row = no_row_selected;
}

void wolfram::clear() {
    engine::clear();
    dehighlight_row();
    _current_row = no_row_selected;
}

void wolfram::action1(size_t row, size_t col) {
    if (row == _current_row)
        _grid.set(row, col, states::FILLED_CURRENT);
    else
        _grid.set(row, col, states::FILLED);
}

void wolfram::action2(size_t row, size_t col) {
    if (row == _current_row)
        _grid.set(row, col, states::EMPTY_CURRENT);
    else
        _grid.set(row, col, states::EMPTY);
}

}  // namespace automaton::engines
