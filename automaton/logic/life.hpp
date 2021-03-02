#pragma once

#include <automaton/base/grid.hpp>

namespace automaton {
namespace logic {

class life_2d : public base_logic {
public:
    void step(base_grid& grid) override;
    void clear() override;
};

} // namespace logic
} // namespace automaton
