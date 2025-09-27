#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class wolfram : public engine {
public:
    wolfram(core::grid& grid, core::surface_type surface, std::uint8_t code);
    std::string description() const override;

    bool step() override;
    void restart() override;
    void clear() override;

    void action1(size_t row, size_t col) override;
    void action2(size_t row, size_t col) override;

private:
    void highlight_row();
    void dehighlight_row();

    std::uint8_t _code;

    static constexpr size_t no_row_selected = -1;
    size_t _current_row = no_row_selected;
};

}  // namespace automaton::engines
