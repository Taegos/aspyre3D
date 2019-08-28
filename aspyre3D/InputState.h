#pragma once
#include <vector>
#include <Windows.h>
#include <unordered_set>

struct InputState {
	bool window_closed = false;
	unsigned char key_down = NULL;
	unsigned char key_up = NULL;
	std::unordered_set<unsigned char> keys_held;
};

struct InputStatePrevTick {
	unsigned char key_down = NULL;
	std::unordered_set<unsigned char> keys_held;
};