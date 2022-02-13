#pragma once
#include <stdafx.h>
#include <DXSampleHelper.h>
#include <Windows.h>
#include <d3dx12.h>
#include <Shader/ShaderVariableType.h>
#include <Resource/Buffer.h>
#include <unordered_map>
#include <optional>
#include <Resource/DescriptorHeapView.h>

using Microsoft::WRL::ComPtr;
class TopAccel;
class Shader {
public:
	struct Property {
		ShaderVariableType type;
		uint spaceIndex;
		uint registerIndex;
		uint arrSize;
	};
protected:
	struct InsideProperty : public Property {
		uint rootSigPos;
		InsideProperty(Property const& p)
			: Property(p) {}
	};
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSig;
	std::unordered_map<std::string, InsideProperty> properties;
	std::optional<InsideProperty> GetProperty(std::string_view str) const;

public:
	Shader(
		std::span<std::pair<std::string, Property> const> properties,
		Device* device);
	Shader(
		std::span<std::pair<std::string, Property> const> properties,
		ComPtr<ID3D12RootSignature>&& rootSig);
	Shader(Shader&& v) = default;
	ID3D12RootSignature* RootSig() const { return rootSig.Get(); }

	bool SetResource(
		std::string_view propertyName,
		ID3D12GraphicsCommandList* cmdList,
		BufferView buffer) const;
	bool SetResource(
		std::string_view propertyName,
		ID3D12GraphicsCommandList* cmdList,
		DescriptorHeapView buffer) const;
	virtual ~Shader() = default;
};
