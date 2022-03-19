#pragma vengine_package vengine_directx
#include <Resource/Texture.h>
#include <DXSampleHelper.h>
#include <DXRuntime/FrameResource.h>
Texture::Texture(
	Device* device,
	uint width,
	uint height,
	DXGI_FORMAT format,
	TextureDimension dimension,
	uint depth,
	uint mip,
	TextureUsage usage,
	D3D12_RESOURCE_STATES resourceState)
	: initState(resourceState),
	  usage(usage),
	  dimension(dimension),
	  Resource(device) {
	D3D12_RESOURCE_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D12_RESOURCE_DESC));
	switch (dimension) {
		case TextureDimension::Cubemap:
		case TextureDimension::Tex2DArray:
		case TextureDimension::Tex2D:
			texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			break;
		case TextureDimension::Tex3D:
			texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			break;
		case TextureDimension::Tex1D:
			texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			break;
	}
	texDesc.Alignment = 0;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.DepthOrArraySize = depth;
	texDesc.MipLevels = mip;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	switch (usage) {
		case TextureUsage::RenderTarget:
			texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			break;
		case TextureUsage::DepthStencil:
			texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			break;
		case TextureUsage::UnorderedAccess:
			texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			break;
		case TextureUsage::GenericColor:
			texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			break;
		default:
			texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			break;
	}

	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_HEAP_PROPERTIES const* propPtr = &prop;
	D3D12_CLEAR_VALUE clearValue;
	if (usage == TextureUsage::DepthStencil) {
		clearValue.DepthStencil.Depth = CLEAR_DEPTH;
		clearValue.DepthStencil.Stencil = CLEAR_STENCIL;
	} else {
		memcpy(clearValue.Color, CLEAR_COLOR, 16);
	}
	clearValue.Format = format;
	ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
		propPtr,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		initState,
		&clearValue,
		IID_PPV_ARGS(&resource)));
}

D3D12_SHADER_RESOURCE_VIEW_DESC Texture::GetColorSrvDesc(uint mipOffset) const {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	auto format = resource->GetDesc();
	srvDesc.Format = format.Format;
	switch (dimension) {
		case TextureDimension::Cubemap:
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = mipOffset;
			srvDesc.TextureCube.MipLevels = format.MipLevels;
			srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			break;
		case TextureDimension::Tex2D:
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = mipOffset;
			srvDesc.Texture2D.MipLevels = format.MipLevels;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		case TextureDimension::Tex1D:
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MipLevels = format.MipLevels;
			srvDesc.Texture1D.MostDetailedMip = mipOffset;
			srvDesc.Texture1D.ResourceMinLODClamp = 0.0f;
			break;
		case TextureDimension::Tex2DArray:
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = mipOffset;
			srvDesc.Texture2DArray.MipLevels = format.MipLevels;
			srvDesc.Texture2DArray.PlaneSlice = 0;
			srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
			srvDesc.Texture2DArray.ArraySize = format.DepthOrArraySize;
			srvDesc.Texture2DArray.FirstArraySlice = 0;
			break;
		case TextureDimension::Tex3D:
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			srvDesc.Texture3D.MipLevels = format.MipLevels;
			srvDesc.Texture3D.MostDetailedMip = mipOffset;
			srvDesc.Texture3D.ResourceMinLODClamp = 0.0f;
			break;
	}
	return srvDesc;
}
D3D12_UNORDERED_ACCESS_VIEW_DESC Texture::GetColorUavDesc(uint targetMipLevel) const {
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	auto desc = resource->GetDesc();
	uint maxLevel = desc.MipLevels - 1;
	targetMipLevel = std::min(targetMipLevel, maxLevel);
	uavDesc.Format = desc.Format;
	switch (dimension) {
		case TextureDimension::Tex2D:
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Texture2D.MipSlice = targetMipLevel;
			uavDesc.Texture2D.PlaneSlice = 0;
			break;
		case TextureDimension::Tex1D:
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
			uavDesc.Texture1D.MipSlice = targetMipLevel;
			break;
		case TextureDimension::Tex3D:
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			uavDesc.Texture3D.FirstWSlice = 0;
			uavDesc.Texture3D.MipSlice = targetMipLevel;
			uavDesc.Texture3D.WSize = desc.DepthOrArraySize >> targetMipLevel;
			break;
		default:
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			uavDesc.Texture2DArray.ArraySize = desc.DepthOrArraySize;
			uavDesc.Texture2DArray.FirstArraySlice = 0;
			uavDesc.Texture2DArray.MipSlice = targetMipLevel;
			uavDesc.Texture2DArray.PlaneSlice = 0;
			break;
	}
	return uavDesc;
}
Texture::Texture(
	Device* device,
	IDXGISwapChain3* swapchain,
	uint frame)
	: Resource(device) {
	initState = D3D12_RESOURCE_STATE_PRESENT;
	dimension = TextureDimension::Tex2D;
	usage = TextureUsage::RenderTarget;
	ThrowIfFailed(swapchain->GetBuffer(frame, IID_PPV_ARGS(&resource)));
}
Texture::~Texture() {
}
void Texture::DelayDispose(FrameResource* frameRes) const {
	frameRes->AddDelayDisposeResource(resource);
}
