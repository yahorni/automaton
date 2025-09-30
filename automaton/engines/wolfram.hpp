#pragma once

#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class wolfram : public engine {
public:
    wolfram(parameters& params, uint8_t code);
    std::string description() const override;

    bool step() override;
    void restart() override;
    void clear() override;

    void action1(size_t row, size_t col) override;
    void action2(size_t row, size_t col) override;
    // TODO: shift action to draw a line, not cells

private:
    void highlight_row();
    void dehighlight_row();

    uint8_t step_cylinder(const core::dims& size, const core::grid_state& state, size_t col);
    uint8_t step_plain(const core::dims& size, const core::grid_state& state, size_t col);

    uint8_t _code;

    static constexpr size_t no_row_selected = -1;
    size_t _current_row = no_row_selected;
};

}  // namespace automaton::engines
