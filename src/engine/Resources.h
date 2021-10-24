#pragma once


#include "visual/renderer/appearence.h"


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
        RFont *arial;
    } font;

    ~ResourcesHolder();

    void init();
};

extern ResourcesHolder Resources;
