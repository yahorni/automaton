#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include "graphic.hpp"

namespace automaton {

class FieldWindow : public Gtk::ApplicationWindow {
   public:
    FieldWindow();
    ~FieldWindow() override;

   private:
    // members
    Gtk::Frame _frame;
    CellArea _area;

    // signals
    bool on_key_press(GdkEventKey* ev);
};

}  // namespace automaton
