#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h>

class FieldWindow : public Gtk::ApplicationWindow {
    Gtk::Button btn;

   public:
    FieldWindow() : btn("Start") {
        btn.signal_clicked().connect([this]() { btn.set_label("Automaton"); });
        add(btn);
        show_all();
    }
};

