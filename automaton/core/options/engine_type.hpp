#pragma once

#include <string_view>

namespace automaton::options::engine {

enum class type {
    WOLFRAM,
    SAND,
    LIFE,
    ANT,

    INVALID
};

constexpr std::string_view wolfram = "wolfram";
constexpr std::string_view sand = "sand";
constexpr std::string_view life = "life";
constexpr std::string_view ant = "ant";
constexpr std::string_view invalid = "invalid";

std::string_view to_string(type value);

type from_string(std::string_view svalue);

bool is_valid(std::string_view svalue);

}  // namespace automaton::options::engine

namespace automaton::core {
using engine_type = options::engine::type;
}  // namespace automaton::core
