#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

#include <vector>

namespace automaton::engines {

class ant : public engine {
public:
    explicit ant(core::grid& grid, core::surface_type surface);
    std::string description() const override;

    void activate(size_t row, size_t col) override;
    void deactivate(size_t row, size_t col) override;

protected:
    bool do_step() override;
    void do_clear() override;

private:
    enum class directions { UP, DOWN, LEFT, RIGHT };

    struct _ant {
        size_t row;
        size_t col;
        directions dir;

        void rotate_and_move_torus(bool is_empty, const core::dims& dims);
        void rotate_and_move_plain(bool is_empty, const core::dims& dims);
    };

    std::vector<_ant> _ants;
};

}  // namespace automaton::engines
