#include "Fatal.h"
#include <Windows.h>

void Fatal::Error(const std::string &message)
{
	MessageBoxA(NULL, message.c_str(), "Fatal Error", MB_ICONERROR);
	exit(-1);
}

//void Fatal::Error(HRESULT hr)
//{
//	_com_error error(hr);
//	MessageBoxA(NULL, error.ErrorMessage(), "Fatal Error", MB_ICONERROR);
//	exit(-1);
//}
