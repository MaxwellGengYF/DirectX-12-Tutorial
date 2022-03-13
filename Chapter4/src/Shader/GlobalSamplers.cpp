
#include <Shader/GlobalSamplers.h>

struct GlobalSampleData {
	std::array<D3D12_STATIC_SAMPLER_DESC, 16> arr;
	GlobalSampleData() {
		D3D12_FILTER filters[] = {
			D3D12_FILTER_MIN_MAG_MIP_POINT,
			D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT,
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_FILTER_ANISOTROPIC};
		D3D12_TEXTURE_ADDRESS_MODE addressMode[] = {
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER};
		memset(arr.data(), 0, sizeof(D3D12_STATIC_SAMPLER_DESC) * arr.size());
		size_t idx = 0;
		for (size_t x = 0; x < 4; ++x)
			for (size_t y = 0; y < 4; ++y) {
				auto&& v = arr[idx];
				v.Filter = filters[y];
				auto address = addressMode[x];
				v.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
				v.AddressU = address;
				v.AddressV = address;
				v.AddressW = address;
				v.MipLODBias = 0;
				v.MaxAnisotropy = 16;
				v.MinLOD = 0;
				v.MaxLOD = 16;
				v.ShaderRegister = idx;
				v.RegisterSpace = 0;
				v.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				++idx;
			}
	}
};
static GlobalSampleData sampleData;

std::span<D3D12_STATIC_SAMPLER_DESC> GlobalSamplers::GetSamplers() {
	return {sampleData.arr.data(), sampleData.arr.size()};
}
