#pragma once
#include <Resource/Resource.h>
enum class TextureDimension : vbyte {
	None,
	Tex1D,
	Tex2D,
	Tex3D,
	Cubemap,
	Tex2DArray,
};
class Texture final : public Resource {
public:
	enum class TextureUsage : uint {
		None = 0,
		RenderTarget = 0x1,
		DepthStencil = 0x2,
		UnorderedAccess = 0x4,
		GenericColor = (0x4 | 0x1) // Both render target and unordered access
	};
	static constexpr float CLEAR_COLOR[4] = {0,0,0,0};
	static constexpr float CLEAR_DEPTH = 1;
	static constexpr uint8_t CLEAR_STENCIL = 0;

private:
	ComPtr<ID3D12Resource> resource;
	D3D12_RESOURCE_STATES initState;
	TextureDimension dimension;
	TextureUsage usage;

public:
	Texture(
		Device* device,
		uint width,
		uint height,
		DXGI_FORMAT format,
		TextureDimension dimension,
		uint depth,
		uint mip,
		TextureUsage usage,
		D3D12_RESOURCE_STATES resourceState);
	Texture(
		Device* device,
		IDXGISwapChain3* swapchain,
		uint frame);
	~Texture();
	ID3D12Resource* GetResource() const override {
		return resource.Get();
	}
	D3D12_RESOURCE_STATES GetInitState() const {
		return initState;
	}
	D3D12_SHADER_RESOURCE_VIEW_DESC GetColorSrvDesc(uint mipOffset) const;
	D3D12_UNORDERED_ACCESS_VIEW_DESC GetColorUavDesc(uint targetMipLevel) const;
	void DelayDispose(FrameResource* frameRes) const override;
};
