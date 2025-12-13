#pragma once

#include <array>

namespace automaton::defaults {

namespace rules {
constexpr auto wolfram = 22;
constexpr auto life = "life";
constexpr auto ant = "LR";
}  // namespace rules

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

namespace palette {
constexpr auto background{"#c0c0c0"};
constexpr auto font{"#ff0000"};
constexpr auto states = std::to_array({
    // https://loading.io/color/feature/Paired-12/
    "#fbfbfb",  // "#a6cee3" - original color, but I don't like it
    "#1f78b4",
    "#33a02c",
    "#b2df8a",
    "#fb9a99",
    "#e31a1c",
    "#fdbf6f",
    "#ff7f00",
    "#cab2d6",
    "#6a3d9a",
    "#ffff99",
    "#b15928",
});
}  // namespace palette

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
constexpr auto max_rows = 700;
constexpr auto max_cols = 700;
constexpr auto adapt_to_window = false;
}  // namespace grid

}  // namespace automaton::defaults
