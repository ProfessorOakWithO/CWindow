#include "x11_input.h"
#include "x11_window.h"
#include <assert.h>

namespace engine
{
    std::array<KeyboardKey, MAX_KEYCODE> KeycodeTranslationTable::keycode_to_engine_key_table = {};
    std::array<MouseButton, MAX_BUTTON> KeycodeTranslationTable::button_to_engine_button_table = {};

    KeycodeTranslationTable::KeycodeTranslationTable()
    {
        // Keycodes represent physical or logical keys and lie in the range [8,255].
        // The x11 evdev driver gets the keycode from the kernel and adds 8 to it.
        // LINUX_KEYCODE + 8 = X11 Keycode
        keycode_to_engine_key_table[9] =  KeyboardKey::KEY_ESCAPE;
        keycode_to_engine_key_table[10] = KeyboardKey::KEY_1;
        keycode_to_engine_key_table[11] = KeyboardKey::KEY_2;
        keycode_to_engine_key_table[12] = KeyboardKey::KEY_3;
        keycode_to_engine_key_table[13] = KeyboardKey::KEY_4;
        keycode_to_engine_key_table[14] = KeyboardKey::KEY_5;
        keycode_to_engine_key_table[15] = KeyboardKey::KEY_6;
        keycode_to_engine_key_table[16] = KeyboardKey::KEY_7;
        keycode_to_engine_key_table[17] = KeyboardKey::KEY_8;
        keycode_to_engine_key_table[18] = KeyboardKey::KEY_9;
        keycode_to_engine_key_table[19] = KeyboardKey::KEY_0;

        keycode_to_engine_key_table[24] = KeyboardKey::KEY_Q;
        keycode_to_engine_key_table[25] = KeyboardKey::KEY_W;
        keycode_to_engine_key_table[26] = KeyboardKey::KEY_E;
        keycode_to_engine_key_table[27] = KeyboardKey::KEY_R;
        keycode_to_engine_key_table[28] = KeyboardKey::KEY_T;
        keycode_to_engine_key_table[29] = KeyboardKey::KEY_Y;
        keycode_to_engine_key_table[30] = KeyboardKey::KEY_U;
        keycode_to_engine_key_table[31] = KeyboardKey::KEY_I;
        keycode_to_engine_key_table[32] = KeyboardKey::KEY_O;
        keycode_to_engine_key_table[33] = KeyboardKey::KEY_P;

        keycode_to_engine_key_table[38] = KeyboardKey::KEY_A;
        keycode_to_engine_key_table[39] = KeyboardKey::KEY_S;
        keycode_to_engine_key_table[40] = KeyboardKey::KEY_D;
        keycode_to_engine_key_table[41] = KeyboardKey::KEY_F;
        keycode_to_engine_key_table[42] = KeyboardKey::KEY_G;
        keycode_to_engine_key_table[43] = KeyboardKey::KEY_H;
        keycode_to_engine_key_table[44] = KeyboardKey::KEY_J;
        keycode_to_engine_key_table[45] = KeyboardKey::KEY_K;
        keycode_to_engine_key_table[46] = KeyboardKey::KEY_L;

        keycode_to_engine_key_table[52] = KeyboardKey::KEY_Z;
        keycode_to_engine_key_table[53] = KeyboardKey::KEY_X;
        keycode_to_engine_key_table[54] = KeyboardKey::KEY_C;
        keycode_to_engine_key_table[55] = KeyboardKey::KEY_V;
        keycode_to_engine_key_table[56] = KeyboardKey::KEY_B;
        keycode_to_engine_key_table[57] = KeyboardKey::KEY_N;
        keycode_to_engine_key_table[58] = KeyboardKey::KEY_M;

        // Mouse
        button_to_engine_button_table[Button1] = MouseButton::LEFT;
        button_to_engine_button_table[Button2] = MouseButton::SCROLL_WHEEL;
        button_to_engine_button_table[Button3] = MouseButton::RIGHT;
    }


    KeyboardKey KeycodeTranslationTable::translate_keycode(KeyCode keycode) const {
        assert(keycode >= MIN_KEYCODE and keycode <= MAX_KEYCODE);
        return keycode_to_engine_key_table[keycode];
    }

    MouseButton KeycodeTranslationTable::translate_button(KeyCode keycode) const {
        return button_to_engine_button_table[keycode];
    }

    KeySym keycode_to_keysym(Display* display, unsigned int keycode, unsigned int state) {
        /*
            This works as follows:
            1. Check if the key has a group count, if not return False (No group count = no keysym)
            2. Get the effective group from @state and if the group is out of range ( group < 0 | group > 3)
            correct it into a valid group ( 0 <= group <= 3)
            3. Get the correct shift level within the group
            5. With a valid effective group and a valid shift level we can now get the keysym from the keysym map.
            5. Return True when there is a keysym and it is not equal to NoSymbol
        */
        unsigned int unused_modifiers{};
        KeySym found_keysym{};

        return (XkbLookupKeySym(display, keycode, state, &unused_modifiers, &found_keysym)) ? found_keysym : NoSymbol;
    }
}
