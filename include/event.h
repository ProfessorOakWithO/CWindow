#ifndef EVENT_H_
#define EVENT_H_

#include "input.h"
#include <variant>

namespace engine {
    
    using Timestamp = unsigned long;

    enum class EventType {
        UnknownEvent,
        KeyEvent,
        MouseMoveEvent,
        MouseClickEvent
    };

    struct KeyEvent {
        KeyboardKey keycode{};
        // Unicode representation. I guess this wastes memory.
        char32_t keysymbol{};
        InputState state = InputState::UNKNOWN;
    };

    struct MouseButtonEvent {
        int x{};
        int y{};
        MouseButton button = MouseButton::UNKNOWN;
        InputState state = InputState::UNKNOWN;
    };

    struct MouseMoveEvent {
        int x{}; 
        int y{};
    };

    struct Event {
        EventType event_type = EventType::UnknownEvent;
        Timestamp event_time{};
        std::variant<std::monostate, KeyEvent, MouseButtonEvent, MouseMoveEvent> data{};
    };
}

#endif