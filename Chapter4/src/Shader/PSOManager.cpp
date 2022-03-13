#include <Shader/PSOManager.h>
#include <Shader/RasterShader.h>
#include <filesystem>
PSOManager::PSOManager(Device* device)
	: device(device) {
}
PSOManager::~PSOManager() {
}
ID3D12PipelineState* PSOManager::GetPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC const& stateDesc) {
	auto emplaceResult = pipelineStates.try_emplace(stateDesc);
	// New pipeline state
	if (emplaceResult.second) {
		ThrowIfFailed(device->DxDevice()->CreateGraphicsPipelineState(&stateDesc, IID_PPV_ARGS(&emplaceResult.first->second)));
	}
	return emplaceResult.first->second.Get();
}
ID3D12PipelineState* PSOManager::GetPipelineState(
	std::span<D3D12_INPUT_ELEMENT_DESC const> meshLayout,
	RasterShader const* shader,
	std::span<DXGI_FORMAT> rtvFormats,
	DXGI_FORMAT depthFormat,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType) {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = {meshLayout.data(), uint(meshLayout.size())};
	psoDesc.pRootSignature = shader->RootSig();
	auto GetByteCode = [](ComPtr<ID3DBlob> const& blob) -> D3D12_SHADER_BYTECODE {
		if (blob) {
			return CD3DX12_SHADER_BYTECODE(blob.Get());
		} else {
			return CD3DX12_SHADER_BYTECODE(nullptr, 0);
		}
	};
	psoDesc.VS = GetByteCode(shader->vsShader);
	psoDesc.PS = GetByteCode(shader->psShader);
	psoDesc.HS = GetByteCode(shader->hsShader);
	psoDesc.DS = GetByteCode(shader->dsShader);
	psoDesc.RasterizerState = shader->rasterizeState;
	psoDesc.DepthStencilState = shader->depthStencilState;
	psoDesc.BlendState = shader->blendState;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = topologyType;
	
	size_t rtSize = std::min<size_t>(rtvFormats.size(), 8);
	psoDesc.NumRenderTargets = rtSize;
	for (size_t i = 0; i < rtSize; ++i) {
		psoDesc.RTVFormats[i] = rtvFormats[i];
	}
	psoDesc.DSVFormat = depthFormat;
	psoDesc.SampleDesc.Count = 1;
	return GetPipelineState(psoDesc);
}