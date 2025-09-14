#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class wolfram : public engine {
public:
    wolfram(core::grid& grid, core::surface_type surface, std::uint8_t code);

    bool step() override;
    void reset() override;
    std::string description() const override;

private:
    std::uint8_t _code;

    size_t _current_row = 0;
};

}  // namespace automaton::engines
