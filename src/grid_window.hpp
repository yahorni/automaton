#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include "grid_area.hpp"

namespace automaton {

class grid_window : public Gtk::ApplicationWindow {
   public:
    grid_window();
    ~grid_window() override;

   private:
    // members
    Gtk::Frame _frame;
    grid_area _area;

    // signals
    bool on_key_press(GdkEventKey* ev);
};

}  // namespace automaton
