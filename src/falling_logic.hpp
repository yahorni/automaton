#pragma once

#include <random>

#include "grid_2d.hpp"
#include "grid_3d.hpp"

namespace automaton {

class falling_logic_2d : public logic_2d {
   public:
    void step(grid_2d& grid) override;
};

class falling_logic_3d : public logic_3d {
   public:
    falling_logic_3d();
    void step(grid_3d& grid) override;

   private:
    std::default_random_engine _engine;
    std::uniform_int_distribution<int16_t> _dist;
};

}  // namespace automaton
