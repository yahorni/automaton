#pragma once

#include <string_view>

namespace automaton::options::surface {

enum class type {
    DEFAULT,

    PLAIN,
    CYLINDER,
    TORUS,

    INVALID
};

constexpr std::string_view default_ = "default";
constexpr std::string_view plain = "plain";
constexpr std::string_view cylinder = "cylinder";
constexpr std::string_view torus = "torus";
constexpr std::string_view invalid = "invalid";

std::string_view to_string(type value);

type from_string(std::string_view svalue);

bool is_valid(std::string_view svalue);

}  // namespace automaton::options::surface

namespace automaton::core {
using surface_type = options::surface::type;
}  // namespace automaton::core
