#include "RenderWindow.h"


RenderWindow::RenderWindow(HINSTANCE h_instance, const std::string &title, int width, int height)
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
		Fatal::Error(GetLastError());		
	}
	ShowWindow(handle, SW_SHOW);
	SetForegroundWindow(handle);
	SetFocus(handle);
}

RenderWindow::~RenderWindow()
{
	UnregisterClass(windows_class.c_str(), h_instance);
	DestroyWindow(handle);
}

InputState RenderWindow::capture_input_state()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.
	InputState input_state;
	std::unordered_set<unsigned char> keys_held = input_state_prev_tick.keys_held;
	if (input_state_prev_tick.key_down != NULL) {
		keys_held.emplace(input_state_prev_tick.key_down);
	}
	if (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		this->handle, //Handle to window we are checking messages for
		0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
		0,    //Maximum Filter Msg Value
		PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
		unsigned char code = static_cast<unsigned char>(msg.wParam);
		switch (msg.message) {
			case WM_NULL: {
				UnregisterClass(windows_class.c_str(), h_instance);
				input_state.window_closed = true;				
			}
			case WM_KEYUP: {
				input_state.key_up = code;
				auto it = keys_held.find(code);
				keys_held.erase(it);
			}
			case WM_KEYDOWN: {
				bool repeat = msg.lParam & 0x40000000;
				if (!repeat) {
					input_state.key_down = code;
				}
			}
		}
	}
	input_state.keys_held = keys_held;
	input_state_prev_tick = { input_state.key_down, input_state.keys_held };
	return input_state;
}

void RenderWindow::register_window_class(HINSTANCE h_instance, const std::string &windows_class)
{
	WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc = DefWindowProc; //Pointer to Window Proc function for handling messages from this window
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
