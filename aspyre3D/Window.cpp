#include "Window.h"
#include <exception>
#include <comdef.h>

Window::Window(HINSTANCE h_instance, const std::string &title, int width, int height) : 
	log { "WINDOW" }
{ 
	this->h_instance = h_instance;
	register_window_class(h_instance, windows_class);
	handle = CreateWindowEx(0, //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
		windows_class.c_str(), //Window class name
		title.c_str(), //Window Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
		0, //Window X Position
		0, //Window Y Position
		width, //Window Width
		height, //Window Height
		NULL, //Handle to parent of this window. Since this is the first window, it has no parent window.
		NULL, //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
		h_instance, //Handle to the instance of module to be used with this window
		nullptr
	); //Param to create window
	if (handle == NULL) {
		_com_error error(GetLastError());		
		log.error() << error.ErrorMessage();
		throw new std::invalid_argument(error.ErrorMessage());
	}
	ShowWindow(handle, SW_SHOW);
	SetForegroundWindow(handle);
	SetFocus(handle);
	log.trace() << "initialized" << std::endl;
}

Window::~Window()
{
	if (!UnregisterClass(windows_class.c_str(), h_instance)) {
		_com_error error(GetLastError());
		log.error() << error.ErrorMessage() << std::endl;
	}
	log.trace() << "destroyed" << std::endl;
}

InputState Window::capture_input_state()
{
	MSG msg;
	InputState input_state;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		handle_message(msg, input_state);
	}
	last_state = input_state;
	return input_state;
}

HWND Window::get_handle()
{
	return handle;
}
void Window::handle_message(MSG &msg, InputState &input_state)
{
	unsigned char code = static_cast<unsigned char>(msg.wParam);
	LPARAM l_param = msg.lParam;
	
	std::unordered_set<unsigned char> keys_held = last_state.keys_held;
	if (last_state.key_down != NULL) {
		keys_held.emplace(last_state.key_down);
	}
	input_state.keys_held = keys_held;

	switch (msg.message) {
		case WM_QUIT: {
			input_state.window_closed = true;
			break;
		}
		case WM_KEYUP: {
			input_state.key_up = code;
			auto it = input_state.keys_held.find(code);
			if (it != input_state.keys_held.end()) {
				input_state.keys_held.erase(it);
			}
			break;
		}
		case WM_KEYDOWN: {
			bool repeat = l_param & 0x40000000;
			if (!repeat) {
				input_state.key_down = code;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		case WM_LBUTTONUP:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		case WM_RBUTTONUP:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		case WM_MBUTTONUP:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			input_state.mouse_x = LOWORD(l_param);
			input_state.mouse_y = HIWORD(l_param);
			input_state.mouse_event = msg.message;
			break;
		}
		default: 
			DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			break;
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Window::register_window_class(HINSTANCE h_instance, const std::string &windows_class)
{
	WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc = (WNDPROC)WindowProc; //Pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
	wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
	wc.hInstance = h_instance; //Handle to the instance that contains the Window Procedure
	wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
	wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
	wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
	wc.lpszClassName = windows_class.c_str(); //Pointer to null terminated string of our class name for this window.
	wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize
	RegisterClassEx(&wc); // Register the class so that it is usable.
}
