#ifndef X11_WINDOW_H
#define X11_WINDOW_H

#include "../internal.h"
#include "x11_input.h"

namespace engine
{

    struct PlatformWindowData
    {  
        Display *display = NULL;
        int screen{};
        ::Window window{};
        GC graphics_context{};
        XSetWindowAttributes window_attributes{};
        KeycodeTranslationTable translation_table {};
        bool ewmh_compliant_wm;
    };

}

#endif