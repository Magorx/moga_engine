#include "Resources.h"


sf::Texture *load_texture(const char *filename) {
    sf::Texture *texture = new sf::Texture();
    if (!texture) {
        printf("can't alloc memory for texture [%s]\n", filename);
    }
    if (!texture->loadFromFile(filename)) {
        printf("can't load texture [%s]\n", filename);
    }

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

#define RES(path) "resources/" path
#define IMG(path) RES("image/" path)
#define BUTTON_IMG(name, path) IMG("button/" name "/" path)


void ResourcesHolder::init() {
    texture.button.basic.idle    = load_texture("resources/image/button.png");
    texture.button.basic.hovered = load_texture("resources/image/button.png");
    texture.button.basic.pressed = load_texture("resources/image/button.png");

    texture.button.close.idle    = load_texture(BUTTON_IMG("close", "idle.png"));
    texture.button.close.hovered = load_texture(BUTTON_IMG("close", "hovered.png"));
    texture.button.close.pressed = load_texture(BUTTON_IMG("close", "pressed.png"));

    texture.button.hide.idle    = load_texture(BUTTON_IMG("hide", "idle.png"));
    texture.button.hide.hovered = load_texture(BUTTON_IMG("hide", "hovered.png"));
    texture.button.hide.pressed = load_texture(BUTTON_IMG("hide", "pressed.png"));

    font.arial = load_font("resources/font/arial.ttf");


    texture.util_bar.basic.close_button = &texture.button.close;
    texture.util_bar.basic.hide_button = &texture.button.hide;

    texture.window.basic.util_bar = &texture.util_bar.basic;
}

ResourcesHolder::~ResourcesHolder() {
    delete texture.button.basic.idle   ;
    delete texture.button.basic.hovered;
    delete texture.button.basic.pressed;

    delete texture.button.close.idle   ;
    delete texture.button.close.hovered;
    delete texture.button.close.pressed;

    delete texture.button.hide.idle   ;
    delete texture.button.hide.hovered;
    delete texture.button.hide.pressed;

    delete font.arial;
}


ResourcesHolder Resources {};
