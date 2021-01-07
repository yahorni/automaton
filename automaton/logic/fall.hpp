#pragma once

#include <automaton/grid_2d.hpp>
#include <automaton/grid_3d.hpp>
#include <random>

namespace automaton {
namespace logic {

class fall_2d : public logic_2d {
   public:
    void step(grid_2d& grid) override;
};

class fall_3d : public logic_3d {
   public:
    fall_3d();
    void step(grid_3d& grid) override;

   private:
    std::default_random_engine _engine;
    std::uniform_int_distribution<int16_t> _dist;
};

}  // namespace logic
}  // namespace automaton
