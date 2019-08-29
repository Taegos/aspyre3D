#include "Console.h"
#include <io.h>
#include <fcntl.h>
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <iterator>
#include <thread>  
#include <chrono>


void Console::create()
{
	AllocConsole();
	//HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	//int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	//FILE *COutputHandle = _fdopen(SystemOutput, "w");

	HANDLE console_error = GetStdHandle(STD_ERROR_HANDLE);
	int system_error = _open_osfhandle(intptr_t(console_error), _O_TEXT);
	FILE *error_handle = _fdopen(system_error, "w");

	////HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	////int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	////FILE *CInputHandle = _fdopen(SystemInput, "r");

	//std::ios::sync_with_stdio(true);
	////freopen
	////freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	////freopen_s(&COutputHandle, "CONOUT$", "w", stdclo);
	freopen_s(&error_handle, "CONOUT$", "w", stderr);

	//std::wcout.clear();
	//std::cout.clear();
	//std::wcerr.clear();
	//std::cerr.clear();
	//std::wcin.clear();
	//std::cin.clear();
	//_fdclo
	//AttachConsole(GetCurrentProcessId());
}