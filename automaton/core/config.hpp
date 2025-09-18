#pragma once

#include "automaton/core/defaults.hpp"
#include "automaton/core/engine_type.hpp"
#include "automaton/core/surface_type.hpp"

#include <cstdint>
#include <string>
#include <tuple>

namespace automaton::core {

struct config {
    struct ui_group {
        double cell_width = defaults::cell_width;
        int animation_pause = defaults::animation_pause_ms;
        bool animation = defaults::enable_animation;
        bool borders = defaults::enable_borders;
    } ui;

    struct automaton_group {
        std::string engine{options::engine::sand};
        std::string surface{options::surface::default_};
        // 0 to fill available space
        int initial_rows = 0;
        // 0 to fill available space
        int initial_cols = 0;
        std::string rule;
    } automaton;

    // return: false with failed options, otherwise true
    std::tuple<bool, std::string> validate() const;

    engine_type get_automaton_engine() const;
    surface_type get_automaton_surface() const;
    std::tuple<std::uint16_t, std::uint16_t> get_life_rule() const;
    std::uint8_t get_wolfram_code() const;

    static std::string get_engine_options();
    static std::string get_surface_options();

private:
    bool _validate_cell_width() const;
    bool _validate_pause() const;

    bool _validate_engine() const;
    bool _validate_surface() const;
    bool _validate_cols() const;
    bool _validate_rows() const;
    bool _validate_rule() const;

    bool _validate_surface_for_engine() const;
};

}  // namespace automaton::core
