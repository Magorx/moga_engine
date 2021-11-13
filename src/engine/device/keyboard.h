#pragma once


#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include <cctype>
#include <cstring>


namespace Keyboard {

    extern const int KEYMAP_SIZE;

enum class Key {
    a            = sf::Keyboard::A,
    b            = sf::Keyboard::B,
    c            = sf::Keyboard::C,
    d            = sf::Keyboard::D,
    e            = sf::Keyboard::E,
    f            = sf::Keyboard::F,
    g            = sf::Keyboard::G,
    h            = sf::Keyboard::H,
    i            = sf::Keyboard::I,
    j            = sf::Keyboard::J,
    k            = sf::Keyboard::K,
    l            = sf::Keyboard::L,
    m            = sf::Keyboard::M,
    n            = sf::Keyboard::N,
    o            = sf::Keyboard::O,
    p            = sf::Keyboard::P,
    q            = sf::Keyboard::Q,
    r            = sf::Keyboard::R,
    s            = sf::Keyboard::S,
    t            = sf::Keyboard::T,
    u            = sf::Keyboard::U,
    v            = sf::Keyboard::V,
    w            = sf::Keyboard::W,
    x            = sf::Keyboard::X,
    y            = sf::Keyboard::Y,
    z            = sf::Keyboard::Z,
    num0         = sf::Keyboard::Num0,
    num1         = sf::Keyboard::Num1,
    num2         = sf::Keyboard::Num2,
    num3         = sf::Keyboard::Num3,
    num4         = sf::Keyboard::Num4,
    num5         = sf::Keyboard::Num5,
    num6         = sf::Keyboard::Num6,
    num7         = sf::Keyboard::Num7,
    num8         = sf::Keyboard::Num8,
    num9         = sf::Keyboard::Num9,
    escape       = sf::Keyboard::Escape,
    lcontrol     = sf::Keyboard::LControl,
    lshift       = sf::Keyboard::LShift,
    lalt         = sf::Keyboard::LAlt,
    lsystem      = sf::Keyboard::LSystem,
    rcontrol     = sf::Keyboard::RControl,
    rshift       = sf::Keyboard::RShift,
    ralt         = sf::Keyboard::RAlt,
    rsystem      = sf::Keyboard::RSystem,
    menu         = sf::Keyboard::Menu,
    lbracket     = sf::Keyboard::LBracket,
    rbracket     = sf::Keyboard::RBracket,
    semicolon    = sf::Keyboard::Semicolon,
    comma        = sf::Keyboard::Comma,
    period       = sf::Keyboard::Period,
    quote        = sf::Keyboard::Quote,
    slash        = sf::Keyboard::Slash,
    backslash    = sf::Keyboard::Backslash,
    tilde        = sf::Keyboard::Tilde,
    equal        = sf::Keyboard::Equal,
    hyphen       = sf::Keyboard::Hyphen,
    space        = sf::Keyboard::Space,
    enter        = sf::Keyboard::Enter,
    backspace    = sf::Keyboard::Backspace,
    tab          = sf::Keyboard::Tab,
    pageup       = sf::Keyboard::PageUp,
    pagedown     = sf::Keyboard::PageDown,
    end          = sf::Keyboard::End,
    home         = sf::Keyboard::Home,
    insert       = sf::Keyboard::Insert,
    del          = sf::Keyboard::Delete,
    add          = sf::Keyboard::Add,
    subtract     = sf::Keyboard::Subtract,
    multiply     = sf::Keyboard::Multiply,
    divide       = sf::Keyboard::Divide,
    left         = sf::Keyboard::Left,
    right        = sf::Keyboard::Right,
    up           = sf::Keyboard::Up,
    down         = sf::Keyboard::Down,
    numpad0      = sf::Keyboard::Numpad0,
    numpad1      = sf::Keyboard::Numpad1,
    numpad2      = sf::Keyboard::Numpad2,
    numpad3      = sf::Keyboard::Numpad3,
    numpad4      = sf::Keyboard::Numpad4,
    numpad5      = sf::Keyboard::Numpad5,
    numpad6      = sf::Keyboard::Numpad6,
    numpad7      = sf::Keyboard::Numpad7,
    numpad8      = sf::Keyboard::Numpad8,
    numpad9      = sf::Keyboard::Numpad9,
    f1           = sf::Keyboard::F1,
    f2           = sf::Keyboard::F2,
    f3           = sf::Keyboard::F3,
    f4           = sf::Keyboard::F4,
    f5           = sf::Keyboard::F5,
    f6           = sf::Keyboard::F6,
    f7           = sf::Keyboard::F7,
    f8           = sf::Keyboard::F8,
    f9           = sf::Keyboard::F9,
    f10          = sf::Keyboard::F10,
    f11          = sf::Keyboard::F11,
    f12          = sf::Keyboard::F12,
    f13          = sf::Keyboard::F13,
    f14          = sf::Keyboard::F14,
    f15          = sf::Keyboard::F15,
    pause        = sf::Keyboard::Pause,
};

    extern bool keymap_pressed[];

    inline void init() {
        memset(keymap_pressed, 0, KEYMAP_SIZE);
    }

    inline void key_down(Key key) {
        keymap_pressed[(int) key] = true;
    }

    inline void key_up(Key key) {
        keymap_pressed[(int) key] = false;
    }

    inline bool is_pressed(Key key) {
        return keymap_pressed[(int) key];
    }

    inline bool is_pressed_shift() {
        return is_pressed(Key::lshift) || is_pressed(Key::rshift);
    }

    inline bool is_pressed_ctrl() {
        return is_pressed(Key::lcontrol) || is_pressed(Key::rcontrol);
    }

    inline bool is_pressed_system() {
        return is_pressed(Key::lsystem) || is_pressed(Key::rsystem);
    }

}


namespace Event {

struct TextEnter {
    uint32_t keycode;

    TextEnter(uint32_t unicode) : keycode(unicode) {}

    bool is_ascii() const {
        return ! (keycode & 0xFF00);
    }

    char ascii() const {
        if(!is_ascii()) return 0;

        return (char) keycode;
    }
    
    uint32_t unicode() const {
        return keycode;
    }

    inline bool is_symbolic() const {
        char c = ascii();
        return isspace(c) || isalpha(c) || isdigit(c) || ispunct(c);
    }
};

struct Key {
    Keyboard::Key code;

    explicit Key(Keyboard::Key code) : code(code) {}
};

struct KeyDown : public Key {
    KeyDown(Keyboard::Key code) : Key(code) {}
};

struct KeyUp : public Key {
    KeyUp(Keyboard::Key code) : Key(code) {}
};

}
