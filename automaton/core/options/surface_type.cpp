#include "automaton/core/options/surface_type.hpp"

namespace automaton::options::surface {

std::string_view to_string(type value) {
    switch (value) {
    case type::DEFAULT:
        return default_;
    case type::PLAIN:
        return plain;
    case type::CYLINDER:
        return cylinder;
    case type::TORUS:
        return torus;
    default:
        return invalid;
    }
}

type from_string(std::string_view svalue) {
    if (svalue == default_)
        return type::DEFAULT;
    else if (svalue == plain)
        return type::PLAIN;
    else if (svalue == cylinder)
        return type::CYLINDER;
    else if (svalue == torus)
        return type::TORUS;
    else
        return type::INVALID;
}

bool is_valid(std::string_view svalue) { return from_string(svalue) != type::INVALID; }

}  // namespace automaton::options::surface
