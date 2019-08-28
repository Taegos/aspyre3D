#pragma once
#include "Fatal.h"
#include "InputState.h"

class RenderWindow
{
public:
	RenderWindow(HINSTANCE, const std::string &, int, int);
	~RenderWindow();
	InputState capture_input_state();
private: 
	void register_window_class(HINSTANCE, const std::string &);
	HWND handle;
	HINSTANCE h_instance;
	const std::string windows_class = "ASPYRE3D";
	InputStatePrevTick input_state_prev_tick;
};

