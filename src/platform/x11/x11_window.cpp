#include "window.h"
#include "x11_window.h"
#include "x11_input.h"
#include "x11_error.h"
#include "../../log.h"
#include <string>
#include <assert.h>

namespace engine
{   
    
    static Display *open_display()
    {
        int event, error, reason = 0;
        int lib_major_in_out = XkbMajorVersion, lib_minor_in_out = XkbMinorVersion;

        int error_flags = XkbOD_BadLibraryVersion | XkbOD_ConnectionRefused | XkbOD_BadServerVersion | XkbOD_NonXkbServer | XkbOD_Success;

        // Display refers to a X Server connection. Every display can have multiple screens.
        Display *display = XkbOpenDisplay((char *)0, &event, &error, &lib_major_in_out, &lib_minor_in_out, &reason);
        
        auto error_string = get_xkb_open_display_error_string(error_flags);
        print_logline("Display::open_display() | XkbOpenDisplay | ", (error_string.empty() ? "Success" : error_string));

        assert(display != NULL);
        assert( (error_flags & reason) == 0);

        return display;
    }

    static bool get_window_property(Display* display, ::Window window, Atom property, Atom type, unsigned char** return_value) {
        Atom returned_property_type;
        int returned_property_format;
        unsigned long returned_number_data_units;
        unsigned long number_of_bytes_remaining_after_return;

        // Get reference to child window
        int status = XGetWindowProperty(display, 
                           window,
                           property,
                           0,
                           1,
                           false,
                           type,
                            &returned_property_type,
                            &returned_property_format,
                            &returned_number_data_units,
                            &number_of_bytes_remaining_after_return,
                            return_value              
                           );

        return status == Success and returned_number_data_units >= 1;
    }

    static bool is_wm_ewmh_compliant(Display* display) {
        Atom supporting_wm_check = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", false);
        /*  
            From the Extended Window Manager Hints Spec.
            https://specifications.freedesktop.org/wm-spec/latest/
            --------------------------------------------
            The Window Manager MUST set this property on the root window to be the ID of a child window created by himself, to indicate that a compliant window manager is active. 
            The child window MUST also have the _NET_SUPPORTING_WM_CHECK property set to the ID of the child window. 
            The child window MUST also have the _NET_WM_NAME property set to the name of the Window Manager.

            Rationale: 
            The child window is used to distinguish an active Window Manager from a stale _NET_SUPPORTING_WM_CHECK property that happens to point to another window. 
            If the _NET_SUPPORTING_WM_CHECK window on the client window is missing or not properly set, clients SHOULD assume that no conforming Window Manager is present.
        */

        ::Window* root_child_window{};
        ::Window* child_window{};

        bool root_success = get_window_property(display, DefaultRootWindow(display), supporting_wm_check, XA_WINDOW, (unsigned char**) &root_child_window);
        bool child_success = get_window_property(display, *root_child_window, supporting_wm_check, XA_WINDOW, (unsigned char**) &child_window);
        
        return root_success and child_success and *root_child_window == *child_window;
    }

    static void set_title(Display* display, ::Window window, std::string const& title) {
        Atom name = XInternAtom(display, "WM_NAME", True);

        if(name == NoValue) {
            print_logline("set_title | Can't get atom \"WM_NAME\".");
            return;
        }

        XChangeProperty(display,
                        window,
                        name,
                        XA_STRING,
                        8,
                        PropModeReplace,
                        (unsigned char const*)title.c_str(),
                        title.length());
    }

    Window::Window(int x, int y, int width, int height, std::string title)
        : platform_data{std::make_unique<PlatformWindowData>()}, 
          x{x}, y{y}, 
          width{width}, height{height}, 
          in_fullscreen_mode{false},
          title{title}
    {
        auto* display = open_display();

        platform_data->display = display;
        platform_data->screen = DefaultScreen(platform_data->display);

        platform_data->window_attributes.background_pixel = WhitePixel(platform_data->display, platform_data->screen);
        platform_data->window_attributes.border_pixel = BlackPixel(platform_data->display, platform_data->screen);
        platform_data->window_attributes.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | FocusChangeMask;

        platform_data->window = XCreateWindow(
            platform_data->display,
            DefaultRootWindow(platform_data->display),
            x, y,                                        // x, y
            width, height,                 // width, height
            100,                                         // border_width
            CopyFromParent, InputOutput, CopyFromParent, // depth, class, visual
            CWBackPixel | CWBorderPixel | CWEventMask,   // valuemask
            &platform_data->window_attributes);        // attributes
        
        print_logline("Window::Window() | XCreateWindow | ", std::to_string(platform_data->window));

        // This only works when the window was not mapped.
        // Otherwise we would need to sent a ClientMessage event to the x server.
        set_title(platform_data->display, platform_data->window, title);
        
        // Check if the window manager complies to the Enhanced Window Manager Hints Spec.
        platform_data->ewmh_compliant_wm = is_wm_ewmh_compliant(platform_data->display);

        print_logline("Window::Window() | WindowManager supports EWMH | ", (platform_data->ewmh_compliant_wm ? "true": "false") );

        XMapRaised(platform_data->display, platform_data->window);
        XClearWindow(platform_data->display, platform_data->window);
        XFlush(platform_data->display);
    }

    void Window::toggle_fullscreen() {
        // TODO 26.06.2022: Grab Keyboard and mouse if we switch to fullscreen.
        // TODO 26.06.2022: How to create "fullscreen" if the window manger doesn't support the EWMH spec ?

        /*
            From the Extended Window Manager Hints Spec.
            https://specifications.freedesktop.org/wm-spec/latest/
            --------------------------------------------
            To change the state of a mapped window, 
            a Client MUST send a _NET_WM_STATE client message to the root window:

            window  = the respective client window
            message_type = _NET_WM_STATE
            format = 32
            data.l[0] = the action, as listed below
            data.l[1] = first property to alter
            data.l[2] = second property to alter
            data.l[3] = source indication
            other data.l[] elements = 0

            _NET_WM_STATE_REMOVE         0;    remove/unset property
            _NET_WM_STATE_ADD            1;    add/set property
            _NET_WM_STATE_TOGGLE         2;    toggle property
        */        
        XEvent event = { ClientMessage };
        event.xclient.window = platform_data->window;
        event.xclient.format = 32;
        event.xclient.message_type = XInternAtom(platform_data->display, "_NET_WM_STATE", False);

        event.xclient.data.l[0] = 2;
        event.xclient.data.l[1] = XInternAtom(platform_data->display, "_NET_WM_STATE_FULLSCREEN", False);
        event.xclient.data.l[2] = 0;
        event.xclient.data.l[3] = 0;
        event.xclient.data.l[4] = 0;

        XSendEvent(platform_data->display, DefaultRootWindow(platform_data->display), False, SubstructureNotifyMask|SubstructureRedirectMask, &event);

        in_fullscreen_mode = !in_fullscreen_mode;

        print_logline("Window::toggle_fullscreen() | Fullscreen mode is now ", (in_fullscreen_mode ? "on" : "off"));
    }

    Window::~Window()
    {
        int result = XCloseDisplay(platform_data->display);
        print_logline("Window::~Window() | XCloseDisplay | ", get_x11_base_error_string(result));        
    };

    bool Window::poll_event(Event& platform_event) {
        int events_available = XPending(platform_data->display);
        if (events_available)
        {
            XEvent event;
            XNextEvent(platform_data->display, &event);
            switch (event.type)
            {
                case ConfigureNotify: {
                    print_logline("Window::poll_event() | ConfigureNotify");
                    XConfigureEvent prop = (XConfigureEvent&)event;
                    this->x = prop.x;
                    this->y = prop.y;
                    this->width = prop.width;
                    this->height = prop.height;
                };
                break;

                case KeyPress: {   
                    auto& key_event = (XKeyEvent&)event;
                    auto keysymbol = keycode_to_keysym(platform_data->display, key_event.keycode, key_event.state);
                    auto engine_key = platform_data->translation_table.translate_keycode(key_event.keycode);
                    platform_event = {EventType::KeyEvent, key_event.time, KeyEvent{.keycode = engine_key, .keysymbol = static_cast<char32_t>(keysymbol), .state = InputState::DOWN }};
                }
                break;

                case KeyRelease: {
                    auto& key_event = (XKeyEvent&)event;
                    auto keysymbol = keycode_to_keysym(platform_data->display, key_event.keycode, key_event.state);
                    auto engine_key = platform_data->translation_table.translate_keycode(key_event.keycode);
                    platform_event = {EventType::KeyEvent, key_event.time, KeyEvent{.keycode = engine_key, .keysymbol = static_cast<char32_t>(keysymbol), .state = InputState::UP }};
                }
                break;

                case MotionNotify: {
                    auto move_event = event.xmotion;
                    platform_event = {EventType::MouseMoveEvent, move_event.time, MouseMoveEvent{.x = move_event.x, .y = move_event.y}};
                }
                break;

                case ButtonPress: {
                    auto& button_event = event.xbutton;
                    auto engine_button = platform_data->translation_table.translate_button(button_event.button);
                    platform_event = {EventType::MouseClickEvent, button_event.time, MouseButtonEvent{.x = button_event.x, .y = button_event.y, .button = engine_button, .state = InputState::DOWN }};
                    print_logline("Window::poll_event() | ButtonPress");
                }
                break;
                
                case ButtonRelease: {
                    auto& button_event = event.xbutton;
                    auto engine_button = platform_data->translation_table.translate_button(button_event.button);
                    platform_event = {EventType::MouseClickEvent, button_event.time, MouseButtonEvent{.x = button_event.x, .y = button_event.y, .button = engine_button, .state = InputState::UP }};
                    print_logline("Window::poll_event() | ButtonRelease");
                }
                break;
                
                // TODO: Grab keyboard/pointer when window has focus but also make it possible to select
                // other windows. Naive implementation with XGrab* in FocusIn event and XUngrab* in FocusOut event causes
                // the X server to crash (infinite loop ?).

                case FocusIn: {
                    // XGrabKeyboard(platform_data->display, platform_data->window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
                    // XGrabPointer(platform_data->display, platform_data->window, False, PointerMotionMask|ButtonPressMask, GrabModeAsync, GrabModeAsync, platform_data->window, None, CurrentTime);
                    print_logline("Window::poll_event() | FocusIn");
                }
                break;

                case FocusOut: {    
                    // XUngrabKeyboard(platform_data->display, CurrentTime);
                    // XUngrabPointer(platform_data->display, CurrentTime);
                    print_logline("Window::poll_event() | FocusOut");
                }
                break;

                default: platform_event = {EventType::UnknownEvent, {}};
                    break;
            }
        }
        return events_available;
    }
}
