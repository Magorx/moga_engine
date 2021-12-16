#include "redactor/plugin_std/plugin_std.hpp"
#include "plugin_interface.h"
#include "abstract_widget.h"

#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

#include <vector>

// ============================================================================ Info

const auto PTYPE = PUPPY::EXTENSION;

const char *PNAME    = "a_name";
const char *PVERSION = "a_vesion";
const char *PAUTHOR  = "an_author";
const char *PDESCR   = "a_description";

// ============================================================================ Resources

// ============================================================================ General

const MyPluginInterface PINTERFACE;

const PUPPY::PluginInfo PINFO =
{
    PUPPY::STD_VERSION, // std_version
    nullptr,            // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    nullptr, // icon
    
    PTYPE
};

const PUPPY::AppInterface *APPI = nullptr;


extern "C" const PUPPY::PluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}
 
// ============================================================================ Logic

class Tickable {
public:
    virtual void tick(double dt) = 0;
};

class Animation : public Tickable {
    std::vector<PUPPY::RenderTarget*> frames;
    size_t idx;

    double time;
    double frame_time;

public:
    Animation(const std::vector<PUPPY::RenderTarget*> &frames_, double frame_time) :
    frames(frames_),
    idx(0),
    time(0),
    frame_time(frame_time)
    {}

    void tick(double dt) {
        time += dt;
        while (time > frame_time) {
            time -= frame_time;
            idx = (idx + 1) % frames.size();
        }
    }

    PUPPY::RenderTarget *get_frame() {
        return frames[idx];
    }
};


class UnitWidget : public AbstractWidget, public Tickable {
protected:
    Vec2f pos;
    Vec2f size;
};


class UFO : public AbstractWidget, public Tickable {
    Animation *animation;

    Vec2f pos;
    Vec2f size;
    Vec2f target_pos;
    float speed;

    PUPPY::WBody targeted_body;
    UFO *cow;

public:

    UFO(const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) :
    AbstractWidget(body, parent),
    animation(nullptr),
    target_pos(0),
    speed(250),
    cow(nullptr)
    {
        set_holder(parent);
        // printf("holder is %p\n", parent);
    }

    void put_cow() {
        // if (cow) { return; }

        PUPPY::WBody body = {get_body().position, 64};
        auto cow_holder = APPI->factory.widget->abstract(body, APPI->get_root_widget());
        cow = new UFO({0, body.size}, cow_holder);
        cow->set_animation(new Animation{
            {APPI->factory.target->from_file("./resources/animation/walker/cow.png")},
            0.5
        });
    }

    void set_animation(Animation *animation_) {
        animation = animation_;
    }

    virtual PUPPY::RenderTarget *get_texture() override { if (animation) return animation->get_frame(); else return nullptr; }

    bool is_near(Vec2f pos) {
        return (this->pos - pos).len() < 5;
    }

    bool is_same_target(const PUPPY::WBody target) {
        return targeted_body.position.x == target.position.x
               && targeted_body.position.y == target.position.y
               && targeted_body.size.x == target.size.x
               && targeted_body.size.y == target.size.y;
    }

    void move_to_target(double dt) {
        Vec2f vel = (target_pos - pos).normal() * speed * dt;
        pos += vel;
        set_position({pos.x, pos.y});
    }

    void update_target(bool force = false) {
        auto windows = APPI->get_windows();
        if (!windows.size()) return;

        for (const auto &window : windows) {
            if (is_same_target(window) && !force) {
                return;
            }
        }

        int idx = rand() % windows.size();
        targeted_body = windows[idx];

        target_pos = {targeted_body.position.x, targeted_body.position.y};
        target_pos.y -= size.y;
    }

    bool target_remains() {
        auto windows = APPI->get_windows();

        for (const auto &window : windows) {
            if (is_same_target(window)) {
                return true;
            }
        }

        return false;
    }

    void tick(double dt) {
        auto walker_body = get_body();
        pos = {walker_body.position.x, walker_body.position.y};
        size = {walker_body.size.x, walker_body.size.y};

        if (!is_near(target_pos) && target_remains()) {
            move_to_target(dt);
        } else {
            if (is_near(target_pos) && target_pos.x) {
                put_cow();
                update_target();
            } else {
                update_target();
            }
        }
        focus();
    }
};

Animation *r_cat = nullptr;
UFO *r_walker = nullptr;

// ============================================================================ Basics

PUPPY::Status MyPluginInterface::init(const PUPPY::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    std::vector<PUPPY::RenderTarget*> frames = {
        APPI->factory.target->from_file("./resources/animation/walker/walker_1.png"),
        APPI->factory.target->from_file("./resources/animation/walker/walker_2.png"),
        // APPI->factory.target->from_file("./resources/animation/walker/walker_3.png"),
        // APPI->factory.target->from_file("./resources/animation/walker/walker_4.png"),
    };
    r_cat = new Animation(frames, 0.2);

    PUPPY::WBody body = {100, 64};
    auto holder = APPI->factory.widget->abstract(body, APPI->get_root_widget());
    r_walker = new UFO({0, body.size}, holder);
    r_walker->set_animation(r_cat);

    APPI->log("[plugin](%s) inited", PINFO.name);
    return PUPPY::OK;
}

PUPPY::Status MyPluginInterface::deinit() const {
    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PUPPY::OK;
}

void MyPluginInterface::show_settings() const {}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const PUPPY::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double dt) const {
    r_cat->tick(dt);

    r_walker->tick(dt);
}

void MyPluginInterface::tool_on_press(const PUPPY::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const PUPPY::Vec2f &/*from*/, const PUPPY::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const PUPPY::Vec2f &/*pos*/) const {}

void MyPluginInterface::effect_apply() const {}

void MyPluginInterface::draw(const PUPPY::Vec2f &pos) const {
    float    size = APPI->get_size();
    PUPPY::RGBA color = APPI->get_color();

    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, PUPPY::COPY);

    delete preview;
}
