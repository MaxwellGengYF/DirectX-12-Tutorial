#pragma once
#include <stdafx.h>
#include <DXRuntime/Device.h>
using Microsoft::WRL::ComPtr;
class FrameResource;
class Resource {
protected:
	Device* device;

public:
	Device* GetDevice() const { return device; }
	Resource(Device* device)
		: device(device) {}
	Resource(Resource&&) = default;
	Resource(Resource const&) = delete;
	virtual ~Resource() = default;
	virtual ID3D12Resource* GetResource() const { return nullptr; }
	virtual D3D12_RESOURCE_STATES GetInitState() const { return D3D12_RESOURCE_STATE_COMMON; }
	virtual void DelayDispose(FrameResource* frameRes) const {}
};
