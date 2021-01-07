#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include <memory>

#include "base/grid.hpp"
#include "grid_area.hpp"
#include "utils/cmdline.hpp"

namespace automaton {

class grid_window : public Gtk::ApplicationWindow {
   public:
    grid_window();
    ~grid_window() override;

   private:
    // GUI members
    Gtk::Frame _frame;
    grid_area _area;

    std::shared_ptr<base_grid> _grid;
    cmdline _options;

    // signals
    bool on_key_press(GdkEventKey* ev);

   public:
    int on_cmdline(const Glib::RefPtr<Gio::ApplicationCommandLine>& cmdline,
                   Glib::RefPtr<Gtk::Application>& app);
};

}  // namespace automaton
