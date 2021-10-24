#pragma once


#include "visual/renderer/gate_type_aliases.h"


struct AnimationResourse {
    std::vector<RTexture*> frames;

    ~AnimationResourse() {
        for (auto frame : frames) {
            delete frame;
        }
    }
};

struct ButtonResources {
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

    ButtonResources *close_button;
    ButtonResources *hide_button;
};

struct res_WindowResources {
    res_UtilityBarResources *util_bar;
};

struct ResourcesHolder {
    struct {
        struct {
            ButtonResources close;
            ButtonResources hide;

            ButtonResources basic;
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
    } texture;

    struct {
        AnimationResourse fan;
        AnimationResourse lightning;
    } animation;

    struct {
        RFont *arial;
    } font;

    ~ResourcesHolder();

    void init();
};

extern ResourcesHolder Resources;
