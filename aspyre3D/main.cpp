//Tutorial 1 Solution 2018-09-07 
#include "Window.h"
#include <iostream>
#include "Graphics.h"
#include "Console.h"
#include "Log.h"
//https://stackoverflow.com/questions/191842/how-do-i-get-console-output-in-c-with-a-windows-program



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Console::create();
	std::ofstream error_log;
	error_log.open("error_log.txt", std::ios::out | std::ios::app);
	Log::setup_streams(std::clog, std::clog, std::clog, error_log, error_log);

	int w = 600;
	int h = 800;
	Window rw { hInstance, "aspyre3D", w, h };
	Graphics gfx{ rw.get_handle(), w, h };

	while (true) {
		InputState state = rw.capture_input_state();
		if (state.window_closed) {
			break;
		}
		Sleep(16);
	}

	return 0;
}