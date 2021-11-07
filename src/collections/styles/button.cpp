#include "button.h"


inline MouseReactionStyle *basic_mouse_reaction_style(
    RTexture *idle,
    RTexture *hovered,
    RTexture *pressed,
    Vec2d transformation = {1, 1}
) {
    return new MouseReactionStyle {
        new AppearenceTexture(idle, transformation),
        new AppearenceTexture(pressed, transformation),
        Resources.create_animation({
            pressed,
            hovered,
        }, 0.1, false, transformation),
        Resources.create_animation({
            idle,
            hovered,
        }, 0.2, false, transformation),
        Resources.create_animation({
            hovered,
            idle,
        }, 0.2, false, transformation),
        nullptr
    };
}


namespace StdStyle {

namespace Button {

    MouseReactionStyle *close() {
        return basic_mouse_reaction_style(
            Resources.texture.button.close.idle,
            Resources.texture.button.close.hovered,
            Resources.texture.button.close.pressed
        );
    }

    MouseReactionStyle *hide() {
        return basic_mouse_reaction_style(
            Resources.texture.button.hide.idle,
            Resources.texture.button.hide.hovered,
            Resources.texture.button.hide.pressed
        );
    }

    MouseReactionStyle *basic() {
        return basic_mouse_reaction_style(
            Resources.texture.button.basic.idle,
            Resources.texture.button.basic.hovered,
            Resources.texture.button.basic.pressed
        );
    }

    MouseReactionStyle *plus() {
        return basic_mouse_reaction_style(
            Resources.texture.button.plus.idle,
            Resources.texture.button.plus.hovered,
            Resources.texture.button.plus.pressed
        );
    }

    MouseReactionStyle *save() {
        return basic_mouse_reaction_style(
            Resources.texture.button.save.idle,
            Resources.texture.button.save.hovered,
            Resources.texture.button.save.pressed
        );
    }

    MouseReactionStyle *rgb() {
        return basic_mouse_reaction_style(
            Resources.texture.button.rgb.idle,
            Resources.texture.button.rgb.hovered,
            Resources.texture.button.rgb.pressed
        );
    }

    namespace Arrow {
        MouseReactionStyle *right() {
            return basic_mouse_reaction_style(
                Resources.texture.button.arrow.right.idle,
                Resources.texture.button.arrow.right.hovered,
                Resources.texture.button.arrow.right.pressed
            );
        }

        MouseReactionStyle *left() {
            return basic_mouse_reaction_style(
                Resources.texture.button.arrow.right.idle,
                Resources.texture.button.arrow.right.hovered,
                Resources.texture.button.arrow.right.pressed,
                {-1, 1}
            );
        }
    }

}

}
