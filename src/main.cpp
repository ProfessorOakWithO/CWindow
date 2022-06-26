#include <iostream>
#include <memory>
#include "window.h"
#include "log.h"

static void handle_keyboard(engine::Window& window, engine::KeyEvent const& key_event) {
	static engine::InputState last_input_state = engine::InputState::UP;
	switch (key_event.keycode) {
		case engine::KeyboardKey::KEY_W: {
			if(key_event.state == engine::InputState::DOWN and last_input_state == engine::InputState::UP) {
				window.toggle_fullscreen();
				print_logline("main.cpp | handle_keyboard | Received keyevent down for 'W'");
				print_logline("main.cpp | handle_keyboard | width: ", window.get_width(), ", height: ", window.get_height());
			}

			last_input_state = key_event.state;
		}
		break;
	}
}

static void handle_mouse_click(engine::Window& window, engine::MouseButtonEvent const& button_event) {
	switch(button_event.button) {
		case engine::MouseButton::LEFT: print_logline("main.cpp | handle_mouse_click | Left mouse button was ", (button_event.state == engine::InputState::DOWN ? "down" : "up") );
		break;
		case engine::MouseButton::RIGHT: print_logline("main.cpp | handle_mouse_click | Right mouse button was ", (button_event.state == engine::InputState::DOWN ? "down" : "up") );
		break;
		case engine::MouseButton::SCROLL_WHEEL: print_logline("main.cpp | handle_mouse_click | Scroll wheel button was ", (button_event.state == engine::InputState::DOWN ? "down" : "up") );
		break;
	}
}

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
#elif defined(__linux__)
int main() {
#endif
	engine::Window w(0, 0, 500, 500, "Hello World");
	bool is_running = true;
	while (is_running) {

		engine::Event event{};
		while (w.poll_event(event)) {
			switch (event.event_type) {
				case engine::EventType::KeyEvent: {
					auto key_event = std::get<engine::KeyEvent>(event.data);
					if (key_event.keycode == engine::KeyboardKey::KEY_ESCAPE) {
						is_running = false;
						break;
					}
					handle_keyboard(w, key_event);
				}
				break;

				case engine::EventType::MouseClickEvent: {
					auto button_event = std::get<engine::MouseButtonEvent>(event.data);
					handle_mouse_click(w, button_event);
				}
				break;

				case engine::EventType::MouseMoveEvent:
				break;

				case engine::EventType::UnknownEvent: print_logline("main.cpp | UnknownEvent");
				break;
			}
		}		
	}
}
