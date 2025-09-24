#include "automaton/engines/sand.hpp"

#include <format>
#include <utility>

namespace automaton::engines {

sand::sand(core::grid& grid, core::surface_type surface)
    : engine(grid, core::engine_type::SAND, surface),
      _re(std::mt19937(std::random_device{}())),
      _dist(std::bernoulli_distribution()) {}

std::string sand::description() const {
    return std::format("sand[surface={},size={},step={}]",  //
                       options::surface::to_string(_surface_type), _grid.size(), _step);
}

/* The implementation isn't perfect, but for now it's OK */
bool sand::step() {
    const core::dims& size = _grid.size();
    const core::grid_state& data = _grid.state();

    for (size_t row = size.rows - 2; row != static_cast<size_t>(-1); --row) {
        for (size_t col = 0; col < size.cols; ++col) {
            if (!data[row][col]) continue;

            switch (static_cast<blocks>(data[row][col])) {
            case blocks::SAND: {
                if (!data[row + 1][col]) {
                    _grid.set(row, col, 0);
                    _grid.set(row + 1, col, 1);
                    continue;
                }

                size_t prev_col, next_col;
                if (_surface_type == core::surface_type::CYLINDER) {
                    prev_col = (col == 0 ? size.cols - 1 : col - 1);
                    next_col = (col == size.cols - 1 ? 0 : col + 1);
                } else {  // core::surface_type::PLAIN
                    prev_col = (col == 0 ? 0 : col - 1);
                    next_col = (col == size.cols - 1 ? col : col + 1);
                }

                bool is_left_free = !data[row + 1][prev_col];
                bool is_right_free = !data[row + 1][next_col] && !data[row][next_col];

                if (is_left_free && is_right_free) {
                    _grid.set(row, col, 0);
                    _grid.set(row + 1, _dist(_re) ? prev_col : next_col, 1);
                } else if (is_left_free) {
                    _grid.set(row, col, 0);
                    _grid.set(row + 1, prev_col, 1);
                } else if (is_right_free) {
                    _grid.set(row, col, 0);
                    _grid.set(row + 1, next_col, 1);
                }
                break;
            }
            default:
                break;
            }
        }
    }

    _step++;
    return true;
}

void sand::action1(size_t row, size_t col) { _grid.set(row, col, std::to_underlying(_block_actions)); }

void sand::shift_actions() {
    auto value = std::to_underlying(_block_actions);
    if (value + 1 == std::to_underlying(blocks::SIZE))
        _block_actions = blocks::SAND;
    else
        _block_actions = static_cast<blocks>(value + 1);
}

}  // namespace automaton::engines
