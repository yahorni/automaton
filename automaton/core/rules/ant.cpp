#include "automaton/core/rules/ant.hpp"

#include "automaton/core/defaults.hpp"

#include <algorithm>
#include <regex>
#include <utility>

namespace automaton::rules {

bool ant::is_valid(const std::string& rule) {
    static const std::regex ruleRegex("^[LRN]{2," + std::to_string(defaults::palette::states.size()) + "}$");
    return std::regex_match(rule, ruleRegex);
}

ant::ant(const std::string& rule) {
    _actions.reserve(rule.size());
    std::transform(rule.cbegin(), rule.cend(), std::back_inserter(_actions), [](char c) {
        if (c == 'L') return action::LEFT;
        if (c == 'R') return action::RIGHT;
        if (c == 'N') return action::FORWARD;
        std::unreachable();
    });
}

ant::action ant::operator[](core::cell_state state) const { return _actions.at(state); }

core::cell_state ant::size() const { return _actions.size(); }

std::string ant::to_string() {
    std::string rule;
    std::transform(_actions.cbegin(), _actions.cend(), std::back_inserter(rule), [](action a) {
        switch (a) {
        case action::LEFT:
            return 'L';
        case action::RIGHT:
            return 'R';
        case action::FORWARD:
            return 'N';
        default:
            std::unreachable();
        }
    });
    return rule;
}

}  // namespace automaton::rules
