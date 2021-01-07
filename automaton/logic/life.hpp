#pragma once

#include <automaton/grid_2d.hpp>

namespace automaton {
namespace logic {

class life_2d : public logic_2d {
   public:
    void step(grid_2d& grid) override;
};

}  // namespace logic
}  // namespace automaton
