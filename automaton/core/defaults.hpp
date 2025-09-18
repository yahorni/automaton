#pragma once

namespace automaton::core::defaults {

// window
constexpr auto window_margin = 4;
constexpr auto window_width = 800;
constexpr auto window_height = 800;

// automaton ui

constexpr auto cell_width = 10;
constexpr auto cell_border_width = 0.5;
constexpr auto enable_borders = false;

constexpr auto enable_animation = false;
constexpr auto animation_pause_ms = 100;
constexpr auto redraw_timeout_ms = 50;

constexpr auto font_size = 16;
constexpr auto font_margin = 2;

// automaton engine
constexpr auto wolfram_code = 22;
constexpr auto life_rule = "life";
constexpr auto grid_max_rows = 640;
constexpr auto grid_max_cols = 360;

}  // namespace automaton::core::defaults
