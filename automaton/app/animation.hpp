#include <glibmm/main.h>

namespace automaton::app {

class controller;

class animation : public sigc::trackable {
public:
    animation(int delay, bool enabled);

    void connect(controller& ctrl);
    void start();
    void stop();

    bool is_running() const;
    int delay() const;
    void set_delay(int delay);

private:
    int _delay;
    const bool _enabled;
    bool _is_running;

    sigc::slot<bool()> _motion_slot;
    sigc::connection _animation_connection;
};

}  // namespace automaton::app
