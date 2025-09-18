#pragma once

#include <format>
#include <string>

namespace automaton::options {

template<typename Option>
std::string join() {
    std::string options;
    int last_valid = static_cast<int>(Option::INVALID) - 1;
    for (int i = 0; i != last_valid; ++i) {
        options += std::format("{}, ", to_string(static_cast<Option>(i)));
    }
    options += std::format("{}", to_string(static_cast<Option>(last_valid)));
    return options;
}

}  // namespace automaton::options
