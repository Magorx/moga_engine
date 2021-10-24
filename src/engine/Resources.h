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

    AVMouseReactionResources *close_button;
    AVMouseReactionResources *hide_button;
};

struct res_WindowResources {
    res_UtilityBarResources *util_bar;
};

struct ResourcesHolder {
    struct {
        struct {
            AVMouseReactionResources close;
            AVMouseReactionResources hide;

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
    } texture;

    struct {
        AnimationResourse fan;
        AnimationResourse lightning_idle;
        AnimationResourse lightning_hover;
    } animation;

    struct {
        RFont *arial;
    } font;

    ~ResourcesHolder();

    void init();
};

extern ResourcesHolder Resources;
