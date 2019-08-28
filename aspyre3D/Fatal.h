#pragma once
#include <string>
#include <Windows.h>

class Fatal
{
public:
	static void Error(const std::string&);
	static void Error(HRESULT);
};

