#include "automaton/engines/sand.hpp"

#include <format>

namespace automaton::engines {

sand::sand(core::grid& grid, core::surface_type surface)
    : engine(core::engine_type::SAND, surface),
      _grid(grid),
      _re(std::mt19937(std::random_device{}())),
      _dist(std::bernoulli_distribution()) {}

/* The implementation isn't perfect, but for now it's OK */
bool sand::step() {
    const core::dims& dims = _grid.dims();
    const core::grid_state& data = _grid.state();

    for (size_t row = dims.rows - 2; row != static_cast<size_t>(-1); --row) {
        for (size_t col = 0; col < dims.cols; ++col) {
            if (!data[row][col]) continue;

            if (!data[row + 1][col]) {
                _grid.remove(row, col);
                _grid.add(row + 1, col);
                continue;
            }

            size_t prev_col, next_col;
            if (_surface_type == core::surface_type::CYLINDER) {
                prev_col = (col == 0 ? dims.cols - 1 : col - 1);
                next_col = (col == dims.cols - 1 ? 0 : col + 1);
            } else {
                prev_col = (col == 0 ? 0 : col - 1);
                next_col = (col == dims.cols - 1 ? col : col + 1);
            }

            bool is_left_free = !data[row + 1][prev_col];
            bool is_right_free = !data[row + 1][next_col] && !data[row][next_col];

            if (is_left_free && is_right_free) {
                _grid.remove(row, col);
                _grid.add(row + 1, _dist(_re) ? prev_col : next_col);
            } else if (is_left_free) {
                _grid.remove(row, col);
                _grid.add(row + 1, prev_col);
            } else if (is_right_free) {
                _grid.remove(row, col);
                _grid.add(row + 1, next_col);
            }
        }
    }

    return true;
}

const std::string& sand::name() const {
    static std::string name = std::format("sand[surface={}]", options::surface::to_string(_surface_type));
    return name;
}

}  // namespace automaton::engines
