#include "automaton/core/rules/life.hpp"

#include <format>
#include <map>
#include <regex>
#include <string>

namespace automaton::rules {

const std::map<std::string_view, std::string_view> life::_presets{
    {"life", "B3/S23"},
    {"maze", "B3/S12345"},
    {"corridors", "B3/S1234"},
    {"mice", "B37/S1234"},
    {"conveyors", "B2378/S1234"},
    {"htree", "B1/S012345678"},
    {"diamoeba", "B35678/S5678"},
    {"assimilation", "B345/S4567"},
    {"lfod", "B2/S0"},
    {"seeds", "B2/S"},
    {"rug", "B234/S"},
    {"majority", "B45678/S5678"},
};

bool life::is_valid(const std::string& rule) {
    if (_presets.contains(rule)) return true;

    static const std::regex henselRegex(R"(^B[0-8]*\/S[0-8]*$)");
    return std::regex_match(rule, henselRegex);
}

life::life(const std::string& rule)
    : _birth_mask(0),
      _survival_mask(0) {

    std::string_view hensel = (_presets.contains(rule) ? _presets.at(rule) : rule);

    size_t i = 0;
    for (; hensel[i] != '/'; ++i) {
        _birth_mask |= (1 << (hensel[i] - '0'));
    }
    for (; i < hensel.size(); ++i) {
        _survival_mask |= (1 << (hensel[i] - '0'));
    }
}

std::string life::to_string() const {
    std::string rule = "B";
    for (int i = 0; i < 9; ++i) {
        if (_birth_mask & (1 << i)) rule += static_cast<char>(i + '0');
    }

    rule += "/S";
    for (int i = 0; i < 9; ++i) {
        if (_survival_mask & (1 << i)) rule += static_cast<char>(i + '0');
    }

    return rule;
}

std::string life::get_preset_names() {
    auto it = _presets.cbegin();
    std::string names = std::format("{}", it->first);
    ++it;
    for (; it != _presets.cend(); ++it) {
        names += std::format(", {}", it->first);
    }
    return names;
}

}  // namespace automaton::rules
