#pragma once


#include "engine/device/keyboard.h"


using Key = Keyboard::Key;


namespace HotkeyBind {

    const Hotkey none       = Key::none;

    const Hotkey brush      = {Key::b, Key::lctrl};
    const Hotkey eraser     = Key::e;
    const Hotkey pipette    = Key::q;

    const Hotkey swap_tools = {Key::x, Key::lctrl};

}
