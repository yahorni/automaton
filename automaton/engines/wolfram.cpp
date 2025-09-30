#include "automaton/engines/wolfram.hpp"

#include "automaton/core/engine_type.hpp"

#include <format>

namespace automaton::engines {

wolfram::wolfram(parameters& params, uint8_t code)
    : engine(core::engine_type::WOLFRAM, params),
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
        uint8_t combination;
        if (_surface_type == core::surface_type::CYLINDER) {
            combination = step_cylinder(size, state, col);
        } else {
            combination = step_plain(size, state, col);
        }

        if ((_code >> combination) & 0b1) _grid.set(_current_row + 1, col, states::FILLED);
    }

    _current_row++;
    highlight_row();

    _step++;
    return true;
}

uint8_t wolfram::step_cylinder(const core::dims& size, const core::grid_state& state, size_t col) {
    size_t prev_col = (col == 0 ? size.cols - 1 : col - 1);
    size_t next_col = (col == size.cols - 1 ? 0 : col + 1);

    return (state[_current_row][prev_col] == FILLED) |      // 0b001
           (state[_current_row][col] == FILLED) << 1 |      // 0b010
           (state[_current_row][next_col] == FILLED) << 2;  // 0b100
}

uint8_t wolfram::step_plain(const core::dims& size, const core::grid_state& state, size_t col) {
    return (col != 0 && state[_current_row][col - 1] == FILLED) |                  // 0b001
           (state[_current_row][col] == FILLED) << 1 |                             // 0b010
           (col != size.cols - 1 && state[_current_row][col + 1] == FILLED) << 2;  // 0b100
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
