#include "../widget_std.h"


namespace P {

class Window : virtual public Widget {
public:
    Window(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void()>;

    virtual void set_show_handler(HandlerType &handler_) = 0;
    virtual HandlerType &get_show_handler() = 0;

    virtual void set_hide_handler(HandlerType &handler_) = 0;
    virtual HandlerType &get_hide_handler() = 0;

    virtual bool set_name(const char *name) = 0;
    virtual const char *get_name() = 0;
};

}
