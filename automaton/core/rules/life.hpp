#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace automaton::rules {

class life {
public:
    using mask_type = uint16_t;

    // doesn't check for duplicates/order/semantics
    static bool is_valid(const std::string& rule);

    // should be already validated
    explicit life(const std::string& rule);

    mask_type birth_mask() { return _birth_mask; }
    mask_type survival_mask() { return _survival_mask; }

    std::string to_string() const;

    static std::string get_preset_names();

private:
    mask_type _birth_mask;
    mask_type _survival_mask;

    static const std::map<std::string_view, std::string_view> _presets;
};

}  // namespace automaton::rules
