#pragma once

#include "automaton/app/canvas.hpp"
#include "automaton/core/config.hpp"
#include "automaton/core/grid.hpp"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include <memory>

namespace automaton::app {

class controller;

class window : public Gtk::ApplicationWindow {
public:
    using app_cli_ptr = Glib::RefPtr<Gio::ApplicationCommandLine>;
    using app_ptr = Glib::RefPtr<Gtk::Application>;

    window();
    ~window() override = default;

    int on_command_line(const app_cli_ptr& cli, const app_ptr& app);

private:
    core::config _config;

    Gtk::Frame _frame;
    canvas _canvas;

    core::grid _grid;
    std::shared_ptr<controller> _ctrl;

    bool _parse_command_line(const app_cli_ptr& cli);
    void _initialize();

    // signals
    bool _on_key_press(GdkEventKey* ev);
};

}  // namespace automaton::app
