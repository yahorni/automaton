#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace automaton::options::life {

// clang-format off
// const std::map<std::string_view, std::string_view, std::less<>> presets{
const std::map<std::string_view, std::string_view> presets{
    {"life",        "B3/S23"},
    {"maze",        "B3/S12345"},
    {"corridors",   "B3/S1234"},
    {"mice",        "B37/S1234"},
    {"conveyors",   "B2378/S1234"},
    {"htree",       "B1/S012345678"},
    {"diamoeba",    "B35678/S5678"},
    {"assimilation","B345/S4567"},
    {"lfod",        "B2/S0"},
    {"seeds",       "B2/S"},
    {"rug",         "B234/S"},
    {"majority",    "B45678/S5678"},
};
// clang-format on

// doesn't check for duplicates/order/semantics
bool is_valid(const std::string& rule);

// should be already validated
std::tuple<uint16_t, uint16_t> from_string(const std::string& rule);

std::string to_string(uint16_t birth_mask, uint16_t survival_mask);

std::string get_preset_names();

}  // namespace automaton::options::life
