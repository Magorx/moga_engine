#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/renderer/appearence.h"


class MogaEngine;


struct AnimationResourse {
    std::vector<RTexture*> frames;

    ~AnimationResourse() {
        for (auto frame : frames) {
            delete frame;
        }
    }
};

struct AVMouseReactionResources {
    RTexture *idle;
    RTexture *hovered;
    RTexture *pressed;
};

struct ToolResources {
    RTexture *idle;
    RTexture *chosen;
    RTexture *hovered;
    RTexture *pressed;
};

struct res_UtilityBarResources {
    RTexture *bar;
    RTexture *underbar;
    RTexture *l_corner;
    RTexture *r_corner;

    AVMouseReactionResources *close_button;
    AVMouseReactionResources *hide_button;
};

struct res_WindowResources {
    res_UtilityBarResources *util_bar;
    RTexture *frame;
};

struct ResourcesHolder {
    MogaEngine *engine;

    std::vector<sf::Texture*> created_textures;
    std::vector<Appearence*> created_apprs;

    struct {
        struct {
            AVMouseReactionResources close;
            AVMouseReactionResources hide;
            AVMouseReactionResources b3d;

            AVMouseReactionResources plus;

            struct {
                AVMouseReactionResources right;
            } arrow;

            AVMouseReactionResources basic;
        } button;

        struct {
            ToolResources brush;
            ToolResources eraser;
        } tool;

        struct {
            res_UtilityBarResources basic;
        } util_bar;

        struct {
            res_WindowResources basic;


        } window;

        RTexture *alpha_blue;
        RTexture *frame_gray;

        RTexture *dot;
    } texture;

    struct {
        RTexture *spectrum;
    } color;

    struct {
        AnimationResourse fan;
        AnimationResourse lightning_idle;
        AnimationResourse lightning_hover;
    } animation;

    struct {
        RFont *arial;
        RFont *montserrat;
        RFont *aseprite;

        RFont *basic;

        struct {
            RColor basic_header;
            RColor basic_menu;
        } color;

        struct {
            int basic_header = 20;
            int basic_menu = 25;
        } size;
    } font;

    ~ResourcesHolder();

    void init(MogaEngine *engine_);

    AppearenceAnimation *create_animation(const std::vector<RTexture*> &frames, double frame_duration, bool looped = false, Vec2d transform = {1, 1}, double time_coef = 1);
    RTexture *create_color(RGBA color);
};

extern ResourcesHolder Resources;
