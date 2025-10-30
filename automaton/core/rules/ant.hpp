#pragma once

#include "automaton/core/grid.hpp"

#include <string>
#include <vector>

namespace automaton::rules {

class ant {
public:
    enum class action : core::cell_state { LEFT, RIGHT, FORWARD };

    static bool is_valid(const std::string& rule);

    // should be already validated
    explicit ant(const std::string& rule);

    action operator[](core::cell_state state) const;
    core::cell_state size() const;

    std::string to_string();

private:
    std::vector<action> _actions;
};

}  // namespace automaton::rules
