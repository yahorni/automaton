#pragma once

namespace automaton::defaults {

constexpr auto window_margin = 4;
constexpr auto window_width = 600;
constexpr auto window_height = 600;
constexpr auto redraw_timeout_ms = 30;

constexpr auto cell_width = 10.0;
constexpr auto min_cell_width = 3.0;
constexpr auto max_cell_width = 30.0;
constexpr auto cell_border_width = 0.5;
constexpr auto enable_borders = false;
constexpr auto enable_status = true;
constexpr auto scale_factor = 1.1;

constexpr auto enable_animation = false;
constexpr auto animation_pause_ms = 100;

constexpr auto font_size = 16;
constexpr auto font_margin = 2;

constexpr auto wolfram_code = 22;
constexpr auto life_rule = "life";

namespace grid {
constexpr auto initial_rows = 100;
constexpr auto initial_cols = 100;
constexpr auto min_rows = 1;
constexpr auto min_cols = 1;
// TODO: calculate max size in runtime for given display size
constexpr auto max_rows = 1280ul;
constexpr auto max_cols = 720ul;
constexpr auto adapt_to_window = false;
}  // namespace grid

}  // namespace automaton::defaults
