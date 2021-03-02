#pragma once

#include <automaton/base/grid.hpp>

namespace automaton {
namespace logic {

class wolfram : public base_logic {
public:
    explicit wolfram(int8_t code);

    void step(base_grid& grid) override;
    void clear() override;

private:
    int8_t _code;
    size_t _current_step = 0;
};

} // namespace logic
} // namespace automaton
