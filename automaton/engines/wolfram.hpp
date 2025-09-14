#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class wolfram : public engine {
public:
    wolfram(core::grid& grid, std::uint8_t code, core::surface_type surface);

    bool step() override;
    void reset() override;
    std::string description() const override;

private:
    core::grid& _grid;
    std::uint8_t _code;
    core::surface_type _surface;

    size_t _current_row = 0;
};

}  // namespace automaton::engines
