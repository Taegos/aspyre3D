#pragma once
#include <Windows.h>
#include <unordered_set>

struct InputState {
	bool window_closed = false;
	
	//Keyboard
	unsigned char key_down = NULL;
	unsigned char key_up = NULL;
	std::unordered_set<unsigned char> keys_held;

	//Mouse
	int mouse_x; 
	int mouse_y;
	UINT mouse_event = NULL;
	
};

