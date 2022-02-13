#include <Shader/RasterShader.h>
RasterShader::RasterShader(
	std::span<std::pair<std::string, Property> const> properties,
	Device* device)
	: Shader(properties, device) {}
RasterShader::RasterShader(
	std::span<std::pair<std::string, Property> const> properties,
	ComPtr<ID3D12RootSignature>&& rootSig)
	: Shader(properties, std::move(rootSig)) {}