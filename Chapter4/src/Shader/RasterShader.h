#pragma once
#include <stdafx.h>
#include <Shader/Shader.h>
class RasterShader : public Shader {
public:
	Microsoft::WRL::ComPtr<ID3DBlob> vsShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> psShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> hsShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> dsShader = nullptr;
	D3D12_RASTERIZER_DESC rasterizeState;
	D3D12_DEPTH_STENCIL_DESC depthStencilState;
	D3D12_BLEND_DESC blendState;
	RasterShader(
		std::span<std::pair<std::string, Property> const> properties,
		Device* device);
	RasterShader(
		std::span<std::pair<std::string, Property> const> properties,
		ComPtr<ID3D12RootSignature>&& rootSig);
};