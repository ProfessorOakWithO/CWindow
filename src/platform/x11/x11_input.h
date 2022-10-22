#ifndef X11_INPUT_H
#define X11_INPUT_H

#include "../internal.h"
#include "input.h"
#include <array>

namespace engine
{
    constexpr unsigned int MIN_KEYCODE = 8;
    constexpr unsigned int MAX_KEYCODE = 255;

    constexpr unsigned int MIN_BUTTON = 1;
    constexpr unsigned int MAX_BUTTON = 3;

    class KeycodeTranslationTable {
    public:
        KeycodeTranslationTable();
        KeyboardKey translate_keycode(KeyCode keycode) const;
        MouseButton translate_button(KeyCode keycode) const;
    private:
        static std::array<KeyboardKey, MAX_KEYCODE> keycode_to_engine_key_table;
        static std::array<MouseButton, MAX_BUTTON> button_to_engine_button_table;
    };

    KeySym keycode_to_keysym(Display*, unsigned int, unsigned int);
}
#endif