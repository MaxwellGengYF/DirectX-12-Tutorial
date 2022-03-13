#pragma once
#include <stdafx.h>
#include <memory>
#include <DirectXMath.h>
#include <string>
namespace rtti {
//Interpret a variable's type in structure
struct VarTypeData {
	enum class ScaleType : vbyte {
		Float,
		Int,
		UInt
	};

	ScaleType scale;
	vbyte dimension;
	uint semanticIndex;
	std::string semantic;
	size_t GetSize() const;
};
class VarTypeBase;
class Struct {
	friend class VarTypeBase;
	std::vector<VarTypeData> variables;

public:
	std::span<VarTypeData const> Variables() const { return variables; }
	size_t structSize = 0;

	Struct();
	Struct(Struct const&) = delete;
	Struct(Struct&&) = default;
	void GetMeshLayout(uint slot, std::vector<D3D12_INPUT_ELEMENT_DESC>& resultVector) const;
};
class VarTypeBase {
	size_t offset;

protected:
	VarTypeBase(VarTypeData&& varData);

public:
	size_t Offset() const { return offset; }
};
template<typename T>
class VarType : public VarTypeBase {
protected:
	VarType(VarTypeData&& varData) : VarTypeBase(std::move(varData)) {}

public:
	T const& Get(void const* structPtr) const {
		size_t ptrNum = reinterpret_cast<size_t>(structPtr);
		return *reinterpret_cast<T const*>(ptrNum + Offset());
	}
	T& Get(void* structPtr) const {
		size_t ptrNum = reinterpret_cast<size_t>(structPtr);
		return *reinterpret_cast<T*>(ptrNum + Offset());
	}
};
template<typename T>
struct Var {};

template<>
struct Var<int32> : public VarType<int32> {
	Var(char const* semantic) : VarType<int32>(VarTypeData{VarTypeData::ScaleType::Int, vbyte(1), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<uint> : public VarType<uint> {
	Var(char const* semantic) : VarType<uint>(VarTypeData{VarTypeData::ScaleType::UInt, vbyte(1), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<float> : public VarType<float> {
	Var(char const* semantic) : VarType<float>(VarTypeData{VarTypeData::ScaleType::Float, vbyte(1), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMFLOAT2> : public VarType<DirectX::XMFLOAT2> {
	Var(char const* semantic) : VarType<DirectX::XMFLOAT2>(VarTypeData{VarTypeData::ScaleType::Float, vbyte(2), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMFLOAT3> : public VarType<DirectX::XMFLOAT3> {
	Var(char const* semantic) : VarType<DirectX::XMFLOAT3>(VarTypeData{VarTypeData::ScaleType::Float, vbyte(3), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMFLOAT4> : public VarType<DirectX::XMFLOAT4> {
	Var(char const* semantic) : VarType<DirectX::XMFLOAT4>(VarTypeData{VarTypeData::ScaleType::Float, vbyte(4), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMUINT2> : public VarType<DirectX::XMUINT2> {
	Var(char const* semantic) : VarType<DirectX::XMUINT2>(VarTypeData{VarTypeData::ScaleType::UInt, vbyte(2), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMUINT3> : public VarType<DirectX::XMUINT3> {
	Var(char const* semantic) : VarType<DirectX::XMUINT3>(VarTypeData{VarTypeData::ScaleType::UInt, vbyte(3), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMUINT4> : public VarType<DirectX::XMUINT4> {
	Var(char const* semantic) : VarType<DirectX::XMUINT4>(VarTypeData{VarTypeData::ScaleType::UInt, vbyte(4), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMINT2> : public VarType<DirectX::XMINT2> {
	Var(char const* semantic) : VarType<DirectX::XMINT2>(VarTypeData{VarTypeData::ScaleType::Int, vbyte(2), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMINT3> : public VarType<DirectX::XMINT3> {
	Var(char const* semantic) : VarType<DirectX::XMINT3>(VarTypeData{VarTypeData::ScaleType::Int, vbyte(3), uint(0), std::string(semantic)}) {}
};
template<>
struct Var<DirectX::XMINT4> : public VarType<DirectX::XMINT4> {
	Var(char const* semantic) : VarType<DirectX::XMINT4>(VarTypeData{VarTypeData::ScaleType::Int, vbyte(4), uint(0), std::string(semantic)}) {}
};
}// namespace rtti