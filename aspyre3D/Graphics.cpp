#include "Graphics.h"
#include <exception>
#include <comdef.h>

Graphics::Graphics(HWND hwnd, int width, int height) : 
	log {"GRAPHICS"}
{
	auto tuple = find_best_adapter();
	//log.info() << "best adapter " << std::get<1>(tuple).Description << " found." << std::endl;
	log.trace() << "initialized" << std::endl;
}

Graphics::~Graphics()
{
	log.info() << "destroyed" << std::endl;
}

std::tuple<IDXGIAdapter*, DXGI_ADAPTER_DESC>  Graphics::find_best_adapter() {
	std::vector<IDXGIAdapter*> adapters = get_all_adapters();
	if (adapters.size() == 0) {
		log.error() << "no adapters found" << std::endl;
	}
	IDXGIAdapter * best_adapter = adapters.back();
	DXGI_ADAPTER_DESC best_desc;
	best_adapter->GetDesc(&best_desc);
	if (adapters.size() == 1) {
		return std::make_tuple(best_adapter, best_desc);
	}
	for (int i = 0; i < adapters.size() - 1; i++) {
		IDXGIAdapter * adapter = adapters[i];
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		if (desc.DedicatedVideoMemory > best_desc.DedicatedVideoMemory) {
			best_adapter = adapter;
			best_desc = desc;
		}
	}
	return std::make_tuple(best_adapter, best_desc);
}

std::vector<IDXGIAdapter*> Graphics::get_all_adapters() {
	IDXGIAdapter * adapter;
	std::vector <IDXGIAdapter*> adapters;
	IDXGIFactory* factory = NULL;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)){
		_com_error error(result);
		log.error() << error.ErrorMessage() << std::endl;
	}
	for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		adapters.push_back(adapter);
	}
	if (factory)
	{
		factory->Release();
	}
	return adapters;
}