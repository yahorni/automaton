#pragma once

#include <cstdint>
#include <string>

namespace automaton::options::rule {

// doesn't check for duplicates/order/semantics
bool is_valid(const std::string& rule);

// should be already validated
std::tuple<std::uint16_t, std::uint16_t> from_string(const std::string& rule);

std::string to_string(std::uint16_t birth_mask, std::uint16_t survival_mask);

}  // namespace automaton::options::rule
