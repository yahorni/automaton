#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class wolfram : public engine {
public:
    wolfram(core::grid& grid, core::surface_type surface, std::uint8_t code);
    std::string description() const override;

protected:
    bool do_step() override;
    void do_restart() override;
    void do_clear() override;

private:
    std::uint8_t _code;

    size_t _current_row = 0;
};

}  // namespace automaton::engines
