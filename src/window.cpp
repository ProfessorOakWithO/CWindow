#include "window.h"
#include <iostream>
/*
* window creation is two parts
* 1. create of events
* 2.handling of events
* events are sent through messages
* messages are sent when an event occurs
* we can get the event by using "getmessage()" and "peekmessage()"
* the getmessage waits until a message is sent
* peekmessage just continues if no message was found
* peekmessage allows you to perform processes inbetween, as opposed to get message
* which only allows us to perform processes when  a message is detected
* 
* to handle events, we use "dispatchmessage"
* 
*/
//event->peekmessage->dispatchmessage->windowproc
//note: windowproc is made yourself. When you dispatch a message, windowproc handles the event.
LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM paramw, LPARAM paraml)
{
	if (msg == WM_CLOSE)
	{
		DestroyWindow(wnd);
	}
	else if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(wnd, msg, paramw, paraml);
}
engine::Window::Window(int x, int y, int width, int height, std::string title)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->title = title;
	hinstance = GetDriverModuleHandle(nullptr);
	const LPCTSTR class_name = "Cengine window";

	WNDCLASS cwnd = {}; //create win32 window object
	cwnd.lpszClassName = "cengine window";
	cwnd.hInstance = this->hinstance; //make windows recognize our window
	cwnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);//load the icon
	cwnd.hCursor = LoadCursor(NULL, IDC_ARROW); //load cursor
	cwnd.lpfnWndProc = WindowProc; //define window behavior

	RegisterClass(&cwnd); //register our window class so we can use it

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	//ws_caption = title bar
	//ws_minimizebox = the minimize box you see in the corner
	//ws_sysmenu = displays previous parameters

	//specify dimensions of window
	RECT dimensions;
	//make window appear in left hand corner of screen
	dimensions.left = this->x;
	dimensions.top = this->y;
	//specify width and height of window
	dimensions.right = this->width + dimensions.left;
	dimensions.bottom = this->height + dimensions.top;

	AdjustWindowRect(&dimensions, style, false);//adjust window

	hwinstance = CreateWindowEx(0, class_name, "title", style,
		dimensions.left, dimensions.top, dimensions.right - dimensions.left,
		dimensions.bottom - dimensions.top, nullptr, nullptr, 
		hinstance, nullptr); //finally create our goddamn window
	ShowWindow(this->hwinstance, SW_SHOW);//draw the window to the screen


}
bool engine::Window::poll_messages()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) //while there are messages in the queue
	{
		if (msg.message == WM_QUIT)
		{
			return false; //terminate application
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}
void engine::Window::run_window()
{
	bool running = true;
	while (running)
	{
		if (!this->poll_messages())
		{
			std::cout << "closing window" << std::endl;
			running = false;
		}
		Sleep(10);
		//render
	}
	delete this;
}
engine::Window::~Window()
{
	LPCTSTR class_name = "cengine window";
	UnregisterClass(class_name, hinstance);
}
