#include "Resources.h"
#include "engine/moga_engine.h"


sf::Texture *load_texture(const char *filename) {
    sf::Texture *texture = new sf::Texture();
    if (!texture) {
        printf("can't alloc memory for texture [%s]\n", filename);
    }
    if (!texture->loadFromFile(filename)) {
        printf("can't load texture [%s]\n", filename);
    }

    texture->setRepeated(true);

    return texture;
}

sf::Font *load_font(const char *filename) {
    sf::Font *font = new sf::Font;
    if (!font) {
        printf("can't alloc memory for font [%s]\n", filename);
    }
    if (!font->loadFromFile(filename)) {
        printf("sad story, can't load font [%s]\n", filename);
    }

    return font;
}

void load_animation(AnimationResourse &res, const std::vector<const char*> &frame_names) {
    for (size_t i = 0; i < frame_names.size(); ++i) {
        res.frames.push_back(new RTexture);
        res.frames[i]->loadFromFile(frame_names[i]);
    }
}

#define RES(path) "resources/" path
#define IMG(path) RES("image/" path)
#define BUTTON_IMG(name, path) IMG("button/" name "/" path)

#define ANM(path) RES("animation/") path "/"


void ResourcesHolder::init(MogaEngine *engine_) {
    engine = engine_;

    color.alpha_blue = load_texture(IMG("color/alpha_blue.png"));

    texture.frame_gray = load_texture(IMG("frame_gray.png"));

    texture.button.basic.idle    = load_texture(BUTTON_IMG("basic", "idle.png"));
    texture.button.basic.hovered = load_texture(BUTTON_IMG("basic", "hovered.png"));
    texture.button.basic.pressed = load_texture(BUTTON_IMG("basic", "pressed.png"));

    texture.button.close.idle    = load_texture(BUTTON_IMG("close", "idle.png"));
    texture.button.close.hovered = load_texture(BUTTON_IMG("close", "hovered.png"));
    texture.button.close.pressed = load_texture(BUTTON_IMG("close", "pressed.png"));

    texture.button.hide.idle    = load_texture(BUTTON_IMG("hide", "idle.png"));
    texture.button.hide.hovered = load_texture(BUTTON_IMG("hide", "hovered.png"));
    texture.button.hide.pressed = load_texture(BUTTON_IMG("hide", "pressed.png"));

    texture.button.b3d.idle    = load_texture(BUTTON_IMG("3d", "idle.png"));
    // texture.button.b3d.hovered = load_texture(BUTTON_IMG("3d", "hovered.png"));
    texture.button.b3d.pressed = load_texture(BUTTON_IMG("3d", "pressed.png"));


    font.arial = load_font("resources/font/arial.ttf");
    font.montserrat = load_font("resources/font/Montserrat.ttf");
    font.aseprite = load_font("resources/font/AsepriteFont.ttf");

    load_animation(animation.fan, {
        ANM("fan") "1.png",
        ANM("fan") "2.png",
        ANM("fan") "3.png",
        ANM("fan") "4.png",
        ANM("fan") "5.png",
        ANM("fan") "6.png",
        ANM("fan") "7.png",
        ANM("fan") "8.png",
    });

    load_animation(animation.lightning_idle, {
        ANM("lightning") "1.png",
        ANM("lightning") "2.png",
        ANM("lightning") "3.png",
        ANM("lightning") "4.png",
        ANM("lightning") "5.png",
        ANM("lightning") "6.png",
        ANM("lightning") "7.png",
        ANM("lightning") "8.png",
        ANM("lightning") "9.png",
        ANM("lightning") "10.png",
        ANM("lightning") "11.png",
        ANM("lightning") "12.png",
        ANM("lightning") "13.png",
        ANM("lightning") "14.png",
        ANM("lightning") "15.png",
        ANM("lightning") "16.png",

    });

    load_animation(animation.lightning_hover, {
        ANM("lightning_hover") "1.png",
        ANM("lightning_hover") "2.png",
        ANM("lightning_hover") "3.png",
        ANM("lightning_hover") "4.png",
        ANM("lightning_hover") "5.png",
        ANM("lightning_hover") "6.png",
        ANM("lightning_hover") "7.png",
        ANM("lightning_hover") "8.png",
        ANM("lightning_hover") "9.png",
        ANM("lightning_hover") "10.png",
        ANM("lightning_hover") "11.png",
        ANM("lightning_hover") "12.png",
        ANM("lightning_hover") "13.png",
        ANM("lightning_hover") "14.png",
        ANM("lightning_hover") "15.png",
        ANM("lightning_hover") "16.png",
    });

    texture.util_bar.basic.close_button = &texture.button.close;
    texture.util_bar.basic.hide_button  = &texture.button.hide;

    texture.util_bar.basic.bar      = load_texture(IMG("util_bar/basic/middle.png"));
    texture.util_bar.basic.l_corner = load_texture(IMG("util_bar/basic/corner.png"));
    texture.util_bar.basic.r_corner = load_texture(IMG("util_bar/basic/corner.png"));

    texture.window.basic.util_bar = &texture.util_bar.basic;
    texture.window.basic.frame = color.alpha_blue;
}

ResourcesHolder::~ResourcesHolder() {
    delete texture.alpha_blue;
    delete texture.frame_gray;

    delete texture.button.basic.idle   ;
    delete texture.button.basic.hovered;
    delete texture.button.basic.pressed;

    delete texture.button.close.idle   ;
    delete texture.button.close.hovered;
    delete texture.button.close.pressed;

    delete texture.button.hide.idle   ;
    delete texture.button.hide.hovered;
    delete texture.button.hide.pressed;

    delete texture.button.b3d.idle   ;
    // delete texture.button.b3d.hovered;
    delete texture.button.b3d.pressed;

    delete font.arial;

    for (auto anim : created_animations) {
        delete anim;
    }
}

AppearenceAnimation *ResourcesHolder::create_animation(const std::vector<RTexture*> &frames, double frame_duration, bool looped, double time_coef) {
    std::vector<RTexture*> *anim_frames = new std::vector<RTexture*>;
    anim_frames->reserve(frames.size());
    for (auto frame : frames) {
        anim_frames->push_back(frame);
    }

    AppearenceAnimation *animation = new AppearenceAnimation(anim_frames, frame_duration, looped, time_coef);
    // created_animations.push_back(animation);

    if (engine) {
        animation->tickable_nonfree = true;
        engine->add_tickable(animation);
    }

    return animation;
}


ResourcesHolder Resources {nullptr, {}, {}, {}, {}, {}};
