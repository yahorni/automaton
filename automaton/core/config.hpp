#pragma once

#include "automaton/core/defaults.hpp"
#include "automaton/core/engine_type.hpp"
#include "automaton/core/surface_type.hpp"

#include <cstdint>
#include <string>
#include <tuple>

namespace automaton::core {

struct config {
    struct grid_group {
        double cell_width = defaults::cell_width;
        bool show_borders = defaults::enable_borders;
        int initial_rows = 0;
        int initial_cols = 0;
        bool adapt_to_window = defaults::grid::adapt_to_window;
    } grid;

    struct automaton_group {
        std::string engine{options::engine::sand};
        std::string surface{options::surface::default_};
        std::string rule;
    } automaton;

    struct animation_group {
        int pause = defaults::animation_pause_ms;
        bool enable = defaults::enable_animation;
    } animation;

    // return: false with failed option, otherwise true
    std::tuple<bool, std::string> validate() const;

    engine_type get_automaton_engine() const;
    surface_type get_automaton_surface() const;
    std::tuple<uint16_t, uint16_t> get_life_rule() const;
    uint8_t get_wolfram_code() const;

    static std::string get_engine_options();
    static std::string get_surface_options();
    static std::string get_life_presets();

private:
    bool _validate_cell_width() const;
    bool _validate_cols() const;
    bool _validate_rows() const;

    bool _validate_engine() const;
    bool _validate_surface() const;
    bool _validate_rule() const;

    bool _validate_pause() const;

    bool _validate_surface_for_engine() const;
};

}  // namespace automaton::core
