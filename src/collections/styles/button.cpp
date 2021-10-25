#include "button.h"

namespace StdStyle {

namespace Button {

    MouseReactionStyle *close() {
        return new MouseReactionStyle {
        new AppearenceTexture(Resources.texture.button.close.idle),
        new AppearenceTexture(Resources.texture.button.close.pressed),
        Resources.create_animation({
            Resources.texture.button.close.pressed,
            Resources.texture.button.close.hovered,
        }, 0.1),
        Resources.create_animation({
            Resources.texture.button.close.idle,
            Resources.texture.button.close.hovered,
        }, 0.2),
        Resources.create_animation({
            Resources.texture.button.close.hovered,
            Resources.texture.button.close.idle,
        }, 0.2),
        nullptr
        };
    }

    MouseReactionStyle *hide() {
        return new MouseReactionStyle {
        new AppearenceTexture(Resources.texture.button.hide.idle),
        new AppearenceTexture(Resources.texture.button.hide.pressed),
        Resources.create_animation({
            Resources.texture.button.hide.pressed,
            Resources.texture.button.hide.hovered,
        }, 0.1),
        Resources.create_animation({
            Resources.texture.button.hide.idle,
            Resources.texture.button.hide.hovered,
        }, 0.2),
        Resources.create_animation({
            Resources.texture.button.hide.hovered,
            Resources.texture.button.hide.idle,
        }, 0.2),
        nullptr
        };
    }

    MouseReactionStyle *basic() {
        return new MouseReactionStyle {
        new AppearenceTexture(Resources.texture.button.basic.idle),
        new AppearenceTexture(Resources.texture.button.basic.pressed),
        Resources.create_animation({
            Resources.texture.button.basic.pressed,
            Resources.texture.button.basic.hovered,
        }, 0.1),
        Resources.create_animation({
            Resources.texture.button.basic.idle,
            Resources.texture.button.basic.hovered,
        }, 0.25),
        Resources.create_animation({
            Resources.texture.button.basic.hovered,
            Resources.texture.button.basic.idle,
        }, 0.25),
        nullptr
        };
    }

}

}
