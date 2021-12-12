#pragma once


#include "collections/views/highlighter.h"
#include "redactor/plugin_std/std.hpp"
#include "redactor/plugins/interface/target.h"


inline ViewBody to_vbody(const P::WBody &body) {
    return {{body.position.x, body.position.y}, {body.size.x, body.size.y}};
}

inline P::WBody to_wbody(const ViewBody &body) {
    return {{(float) body.position.x(), (float) body.position.y()}, {(float) body.size.x(), (float) body.size.y()}};
}


#define WIDGET_FIX_(WTYPE)                                \
virtual void set_active(bool flag) override {             \
    if (flag) {                                           \
        widget->on_show({});                              \
    } else {                                              \
        widget->on_hide({});                              \
    }                                                     \
                                                          \
    v_Highlighter::set_active(flag);                      \
}                                                         \
                                                          \
WTYPE *get_widget() { return widget; }                    \
void set_widget(WTYPE *widget_) { widget = widget_; }


class WidgetView : public v_Highlighter {
    P::Widget *widget;

public:
    WidgetView(const ViewBody &body, P::Widget *widget);

    WIDGET_FIX_(P::Widget)
};


template <typename VIEW_T>
class FwdRenderCall : public EventAcceptor<VIEW_T, Event::RenderCall> {
public:
    FwdRenderCall(VIEW_T *widget) : EventAcceptor<VIEW_T, Event::RenderCall>(widget) {}
    virtual EventAccResult operator()(const Event::RenderCall &event, const EventAccResult * = nullptr) override {
        auto acceptor = EventAcceptor<VIEW_T, Event::RenderCall>::acceptor;
        auto widget = acceptor->get_widget();
        widget->on_render({});
        auto texture = dynamic_cast<Target*>(widget->get_texture());

        if (texture) {
            AppearenceTexture appr(texture->get_layer()->get_texture(), acceptor->get_body().size);
            appr.fit_for_size(acceptor->get_body().size);
            event.renderer->set_appearence(&appr);
            event.renderer->apr_draw_rectangle(acceptor->get_body().position, acceptor->get_body().size);

            return EventAccResult::stop;
        }

        return EventAccResult::none;
    }
};


#define FWD_TO_WIDGET_(WTYPE, EVENT_T, METHOD, ARGS)                                                                   \
class WFwd##EVENT_T##WTYPE : public EventAcceptor<WTYPE, Event::EVENT_T> {                                             \
public:                                                                                                                \
    WFwd##EVENT_T##WTYPE(WTYPE *widget) : EventAcceptor<WTYPE, Event::EVENT_T>(widget) {}                                                     \
                                                                                                                       \
    virtual EventAccResult operator()(const Event::EVENT_T &event, const EventAccResult * = nullptr) override {        \
        auto widget = EventAcceptor<WTYPE, Event::EVENT_T>::acceptor->get_widget(); if (!widget) return EventAccResult::none; \
                                                                                                                       \
        widget->METHOD ARGS;                                                                                           \
                                                                                                                       \
        return EventAccResult::none;                                                                                   \
    }                                                                                                                  \
};

#define FWD_CLASS_(WTYPE, EVENT_T) WFwd##EVENT_T##WTYPE(this)

#define FWD_ALL_FOR_CLASS_(WTYPE)                             \
FWD_TO_WIDGET_(WTYPE, MousePress, on_mouse_press, ({          \
    {(float) event.position.x(), (float) event.position.y()}, \
    (P::MouseButton) event.button                             \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, MouseRelease, on_mouse_release, ({      \
    {(float) event.position.x(), (float) event.position.y()}  \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, MouseMove, on_mouse_move, ({            \
    {(float) event.from.x(), (float) event.from.y()},         \
    {(float) event.to.x()  , (float) event.to.y()},           \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, KeyDown, on_key_down, ({                \
    (P::Keyboard::Key) event.code                             \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, KeyUp, on_key_up, ({                    \
    (P::Keyboard::Key) event.code                             \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, TextEnter, on_text_enter, ({            \
    (P::Event::TextEnter) event.keycode                       \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, Scroll, on_scroll, ({                   \
    {(float) event.delta.x(), (float) event.delta.y()},       \
    {(float) event.position.x(), (float) event.position.y()}  \
}))


#define INIT_FWD_TO_WIDGET(WTYPE)                          \
e_render_call.pop_observer();                              \
e_render_call.add(new FwdRenderCall<WTYPE>(this));         \
e_render_call.add(new AVRenderCallAcceptor(this));         \
                                                           \
                                                           \
e_mouse_press.add   (new FWD_CLASS_(WTYPE, MousePress));   \
e_mouse_release.add (new FWD_CLASS_(WTYPE, MouseRelease)); \
e_mouse_move.add    (new FWD_CLASS_(WTYPE, MouseMove));    \
e_key_down.add      (new FWD_CLASS_(WTYPE, KeyDown));      \
e_key_up.add        (new FWD_CLASS_(WTYPE, KeyUp));        \
e_text_enter.add    (new FWD_CLASS_(WTYPE, TextEnter));    \
e_scroll.add        (new FWD_CLASS_(WTYPE, Scroll))


class PluginWidget : virtual public P::Widget {
protected:
    v_Highlighter *view;

public:
    PluginWidget(const ViewBody &body, P::Widget *parent = nullptr, bool to_spawn_view = false);
    PluginWidget(const ViewBody &body, PluginWidget *parent = nullptr, bool to_spawn_view = false);
    PluginWidget(Widget *widget);
    virtual ~PluginWidget();

    v_Highlighter *get_view() { return view; }

    virtual bool is_active() override;
    virtual bool is_inside(P::Vec2f pos) override;

    virtual bool add_child(Widget *child) override;
    virtual bool delete_child(Widget *child) override;

    virtual void set_to_delete(bool to_delete_) override { view->set_to_delete(to_delete_); }

    bool add_child(PluginWidget *child);
    bool delete_child(PluginWidget *child);
    virtual bool delete_from_parent() override;
    
    virtual void on_render          (const P::Event::Render          &) override {}
    virtual void on_tick            (const P::Event::Tick            &) override {}
    virtual void on_mouse_press     (const P::Event::MousePress      &) override {}
    virtual void on_mouse_release   (const P::Event::MouseRelease    &) override {}
    virtual void on_mouse_move      (const P::Event::MouseMove       &) override {}
    virtual void on_key_down        (const P::Event::KeyDown         &) override {}
    virtual void on_key_up          (const P::Event::KeyUp           &) override {}
    virtual void on_text_enter      (const P::Event::TextEnter       &) override {}
    virtual void on_scroll          (const P::Event::Scroll          &) override {}
    virtual void on_hide            (const P::Event::Hide            &) override {}
    virtual void on_show            (const P::Event::Show            &) override {}

    virtual void hide() override;
    virtual void show() override;

    virtual void set_caption(const char *text, size_t font_size, const P::Vec2f *pos = nullptr) override;
    virtual void set_base_color(P::RGBA color) override;
};
