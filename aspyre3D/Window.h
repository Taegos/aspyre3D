#pragma once
#include "Fatal.h"
#include "InputState.h"
#include "Log.h"

class Window
{
public:
	Window(HINSTANCE, const std::string &, int, int);
	~Window();
	InputState capture_input_state();
	HWND get_handle();
private: 
	void register_window_class(HINSTANCE, const std::string &);
	void handle_message(MSG &msg, InputState &);
	HWND handle;
	HINSTANCE h_instance;
	const std::string windows_class = "ASPYRE3D";
	InputState last_state;
	Log log;
};

