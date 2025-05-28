#pragma once

#include "automaton/app/app_config.hpp"
#include "automaton/app/grid_area.hpp"
#include "automaton/grid/base_grid.hpp"
#include "automaton/logic/base_logic.hpp"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include <string>

namespace automaton {

class app_window : public Gtk::ApplicationWindow {
public:
    using app_cli_ptr = Glib::RefPtr<Gio::ApplicationCommandLine>;
    using app_ptr = Glib::RefPtr<Gtk::Application>;

    app_window();
    ~app_window() override = default;

private:
    // GUI members
    Gtk::Frame _frame;
    grid_area _area;

    base_logic_ptr _logic;
    base_grid_ptr _grid;

    app_config _options;

    // signals
    bool on_key_press(GdkEventKey* ev);
    void on_resize();

    // methods
    void initialize_grid();
    std::string get_status() const;

public:
    int on_cli(const app_cli_ptr& cli, app_ptr& app);
};

}  // namespace automaton
