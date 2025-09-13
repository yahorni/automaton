#include "automaton/core/engine_type.hpp"

namespace automaton::options::engine {

std::string_view to_string(type value) {
    switch (value) {
    case type::WOLFRAM:
        return wolfram;
    case type::SAND:
        return sand;
    case type::LIFE:
        return life;
    default:
        return invalid;
    }
}

type from_string(std::string_view svalue) {
    if (svalue == wolfram)
        return type::WOLFRAM;
    else if (svalue == sand)
        return type::SAND;
    else if (svalue == life)
        return type::LIFE;
    else
        return type::INVALID;
}

bool is_valid(std::string_view svalue) { return from_string(svalue) != type::INVALID; }

}  // namespace automaton::options::engine
