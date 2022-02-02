
#include <DXRuntime/Device.h>
#include <DXSampleHelper.h>
Device::~Device() {
}

Device::Device() {
	using Microsoft::WRL::ComPtr;
	uint32_t dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));
	uint adapterIndex = 0;	  // we'll start looking for directx 12  compatible graphics devices starting at index 0
	bool adapterFound = false;// set this to true when a good one was found
	while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0) {
			HRESULT hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1,
										   IID_PPV_ARGS(&dxDevice));
			if (SUCCEEDED(hr)) {
				adapterFound = true;
				break;
			}
		}
		adapter = nullptr;
		adapterIndex++;
	}
}
