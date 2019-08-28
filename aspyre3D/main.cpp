//Tutorial 1 Solution 2018-09-07 
#include "RenderWindow.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	RenderWindow rw{ hInstance, "title", 600, 800 };
	while (true) {
		InputState state = rw.capture_input_state();
		if (state.window_closed) {
			break;
		}
		std::string out;
		if (state.key_down != NULL) {
			out += "down: ";
			out += state.key_down;
			out += "\n";	
		}

		for (unsigned char key_held : state.keys_held) {
			out += key_held;
			if (out.size() >= 80) {
				out += "\n";
			}
		}
		
		if (state.key_up != NULL) {
			out += "\n";
			out += "up: ";
			out += state.key_up;
			out += "\n";
		}
		if (out != "") {
			OutputDebugStringA(out.c_str());
		}
	}
	return 0;
}