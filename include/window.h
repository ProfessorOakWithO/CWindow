//#ifndef WINDOW_H unecessary
//#define WINDOW_H unecessary

#include <memory>
#include "event.h"
#include <Windows.h>
#include <string> 
namespace engine
{
   // class PlatformWindowData; unused for the moment

    class Window
    {
    public:
        Window(int x, int y, int width, int height, std::string title);
        ~Window();
        //might delete copy constructor, ill think about it
        int get_x() const { return x; }
        int get_y() const { return y; }
        int get_width() const { return width; }
        int get_height() const { return height; }
        bool get_in_fullscreen_mode() const { return in_fullscreen_mode; }
        std::string_view get_title() const { return title; }

        //void toggle_fullscreen(); unused for the moment

        //bool poll_event(Event &); unused for the moment
        bool poll_messages();
        void run_window();

    private:
       // std::unique_ptr<PlatformWindowData> platform_data{}; unused for the moment
        int x;
        int y;
        int width;
        int height;
        bool in_fullscreen_mode;
        std::string title;
        HINSTANCE hinstance; //connected to application
        HWND hwinstance; //connected to window
    };
}

//#endifunecessary