#pragma once


#include "collections/views/highlighter.h"
#include "redactor/plugin_std/std.hpp"


class WidgetView : public v_Highlighter {
    P::Widget *widget;

public:
    WidgetView(const ViewBody &body, P::Widget *widget);

    virtual void render(Renderer *renderer) override; 
};


class PluginWidget : public P::Widget {
    v_Highlighter *view;

public:
    PluginWidget(const ViewBody &body, PluginWidget *parent = nullptr);
    PluginWidget(Widget *widget);
    virtual ~PluginWidget();

    v_Highlighter *get_view() { return view; }

    virtual bool is_active() override;
    virtual bool is_inside(P::Vec2f pos) override;

    virtual bool add_child(Widget *child) override;
    virtual bool delete_child(Widget *child) override;

    virtual bool delete_from_parent() override;
    
    virtual void on_render          (Widget *self, const P::Event::Render          &event) override {}
    virtual void on_tick            (Widget *self, const P::Event::Tick            &event) override {}
    virtual void on_mouse_press     (Widget *self, const P::Event::MousePress      &event) override {}
    virtual void on_mouse_release   (Widget *self, const P::Event::MouseRelease    &event) override {}
    virtual void on_mouse_move      (Widget *self, const P::Event::MouseMove       &event) override {}
    virtual void on_key_down        (Widget *self, const P::Event::KeyDown         &event) override {}
    virtual void on_key_up          (Widget *self, const P::Event::KeyUp           &event) override {}
    virtual void on_text_enter      (Widget *self, const P::Event::TextEnter       &event) override {}
    virtual void on_scroll          (Widget *self, const P::Event::Scroll          &event) override {}
    virtual void on_hide            (Widget *self, const P::Event::Hide            &event) override {}
    virtual void on_show            (Widget *self, const P::Event::Show            &event) override {}

    virtual void hide() override;
    virtual void show() override;

    virtual void set_caption(const char *text, size_t font_size, const P::Vec2f *pos = nullptr) override;
};
