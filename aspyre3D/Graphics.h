#pragma once
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>
#include <tuple>
#include "Log.h"

class Graphics
{
public:
	Graphics(HWND, int, int);
	~Graphics();
private:
	std::vector<IDXGIAdapter*> get_all_adapters();
	std::tuple<IDXGIAdapter*, DXGI_ADAPTER_DESC> find_best_adapter();
	Log log;
};

