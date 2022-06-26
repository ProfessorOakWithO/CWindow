#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include "event.h"

namespace engine
{
    class PlatformWindowData;

    class Window
    {
    public:
        Window(int x, int y, int width, int height, std::string title);
        ~Window();

        int get_x() const { return x; }
        int get_y() const { return y; }
        int get_width() const { return width; }
        int get_height() const { return height; }
        bool get_in_fullscreen_mode() const { return in_fullscreen_mode; }
        std::string_view get_title() const { return title; }

        void toggle_fullscreen();

        bool poll_event(Event &);

    private:
        std::unique_ptr<PlatformWindowData> platform_data{};
        int x;
        int y;
        int width;
        int height;
        bool in_fullscreen_mode;
        std::string title;
    };
}

#endif