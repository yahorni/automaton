#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include <memory>

#include "grid_area.hpp"
#include "interfaces/base_grid.hpp"

namespace automaton {

class grid_window : public Gtk::ApplicationWindow {
   public:
    explicit grid_window(std::shared_ptr<base_grid> grid);
    ~grid_window() override;

   private:
    // GUI members
    Gtk::Frame _frame;
    grid_area _area;

    // signals
    bool on_key_press(GdkEventKey* ev);
};

}  // namespace automaton
