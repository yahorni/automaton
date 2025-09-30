#include "automaton/core/life_rule.hpp"

#include <cstdint>
#include <format>
#include <regex>
#include <string>

namespace automaton::options::life {

bool is_valid(const std::string& rule) {
    if (presets.contains(rule)) return true;

    std::regex henselRegex(R"(^B[0-8]*\/S[0-8]*$)");
    return std::regex_match(rule, henselRegex);
}

std::tuple<uint16_t, uint16_t> from_string(const std::string& rule) {
    std::string_view hensel = (presets.contains(rule) ? presets.at(rule) : rule);

    uint16_t birth = 0, survival = 0;
    size_t i = 0;
    for (; hensel[i] != '/'; ++i) {
        birth |= (1 << (hensel[i] - '0'));
    }
    for (; i < hensel.size(); ++i) {
        survival |= (1 << (hensel[i] - '0'));
    }
    return std::tie(birth, survival);
}

std::string to_string(uint16_t birth_mask, uint16_t survival_mask) {
    std::string rule = "B";
    for (int i = 0; i < 9; ++i) {
        if (birth_mask & (1 << i)) rule += static_cast<char>(i + '0');
    }

    rule += "/S";
    for (int i = 0; i < 9; ++i) {
        if (survival_mask & (1 << i)) rule += static_cast<char>(i + '0');
    }

    return rule;
}

std::string get_preset_names() {
    auto it = presets.cbegin();
    std::string names = std::format("{}", it->first);
    ++it;
    for (; it != presets.cend(); ++it) {
        names += std::format(", {}", it->first);
    }
    return names;
}

}  // namespace automaton::options::life
