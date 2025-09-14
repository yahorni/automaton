#include "automaton/core/rule_option.hpp"

#include <cstdint>
#include <regex>
#include <string>

namespace automaton::options::rule {

bool is_valid(const std::string& rule) {
    std::regex henselRegex(R"(^B[0-8]*\/S[0-8]*$)");
    return std::regex_match(rule, henselRegex);
}

std::tuple<std::uint16_t, std::uint16_t> from_string(const std::string& rule) {
    std::uint16_t birth = 0, survival = 0;
    size_t i = 0;
    for (; rule[i] != '/'; ++i) {
        birth |= (1 << (rule[i] - '0'));
    }
    for (; i < rule.size(); ++i) {
        survival |= (1 << (rule[i] - '0'));
    }
    return std::tie(birth, survival);
}

std::string to_string(std::uint16_t birth_mask, std::uint16_t survival_mask) {
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

}  // namespace automaton::options::rule
