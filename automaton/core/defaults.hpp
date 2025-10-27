#pragma once

namespace automaton::defaults {

constexpr auto wolfram_code = 22;
constexpr auto life_rule = "life";

namespace window {
constexpr auto margin = 4;
constexpr auto width = 600;
constexpr auto height = 600;
constexpr auto redraw_delay_ms = 15;
}  // namespace window

namespace ui {
constexpr auto enable_borders = false;
constexpr auto enable_status = true;
}  // namespace ui

namespace cell {
constexpr auto width = 10.0;
constexpr auto min_width = 3.0;
constexpr auto max_width = 30.0;
constexpr auto precision = 100;
constexpr auto border_width = 0.5;
constexpr auto scale_factor = 1.1;
constexpr auto width_delta = 0.1;
}  // namespace cell

namespace animation {
constexpr auto enable = false;
constexpr auto delay_ms = 100;
}  // namespace animation

namespace font {
constexpr auto size = 16;
constexpr auto margin = 2;
}  // namespace font

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
