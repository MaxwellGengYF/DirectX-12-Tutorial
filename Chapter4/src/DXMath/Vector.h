//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard
//

#pragma once
#include <DXMath/Scalar.h>
#include <DXMath/Swizzle.h>
using namespace DirectX;
namespace Math {
struct bool4;
struct bool3 {
	bool x, y, z;
	inline bool3(bool x, bool y, bool z) noexcept : x(x), y(y), z(z) {
	}
	inline bool3(bool x) noexcept : x(x), y(x), z(x) {}
	inline bool3(bool4 b) noexcept;
	inline void operator=(bool4 b) noexcept;
	inline void operator=(bool3 b) noexcept {
		x = b.x;
		y = b.y;
		z = b.z;
	}
	inline bool3 operator||(bool3 b) noexcept {
		return bool3(b.x || x, b.y || y, b.z || z);
	}
	inline bool3 operator^(bool3 b) noexcept {
		return bool3(b.x ^ x, b.y ^ y, b.z ^ z);
	}
	inline bool3 operator&(bool3 b) noexcept {
		return bool3(b.x & x, b.y & y, b.z & z);
	}
	inline bool3 operator|(bool3 b) noexcept {
		return bool3(b.x | x, b.y | y, b.z | z);
	}
	inline bool3 operator&&(bool3 b) noexcept {
		return bool3(b.x && x, b.y && y, b.z && z);
	}
	inline bool3 operator==(bool3 b) noexcept {
		return bool3(b.x == x, b.y == y, b.z == z);
	}
	inline bool3 operator!=(bool3 b) noexcept {
		return bool3(b.x != x, b.y != y, b.z != z);
	}
	inline bool3 operator!() noexcept {
		return bool3(!x, !y, !z);
	}
};

struct bool4 {
	bool x, y, z, w;

	inline bool4(bool x, bool y, bool z, bool w) noexcept : x(x), y(y), z(z), w(w) {}
	inline bool4(bool x) noexcept : x(x), y(x), z(x), w(x) {}
	inline bool4(bool3 b) noexcept : x(b.x), y(b.y), z(b.z), w(false) {
	}
	inline void operator=(bool4 b) noexcept {
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}

	inline void operator=(bool3 b) noexcept {
		x = b.x;
		y = b.y;
		z = b.z;
		w = false;
	}
	inline bool4 operator||(bool4 b) noexcept {
		return bool4(b.x || x, b.y || y, b.z || z, b.w || w);
	}
	inline bool4 operator^(bool4 b) noexcept {
		return bool4(b.x ^ x, b.y ^ y, b.z ^ z, b.w ^ w);
	}
	inline bool4 operator&(bool4 b) noexcept {
		return bool4(b.x & x, b.y & y, b.z & z, b.w & w);
	}
	inline bool4 operator|(bool4 b) noexcept {
		return bool4(b.x | x, b.y | y, b.z | z, b.w | w);
	}
	inline bool4 operator&&(bool4 b) noexcept {
		return bool4(b.x && x, b.y && y, b.z && z, b.w && w);
	}
	inline bool4 operator==(bool4 b) noexcept {
		return bool4(b.x == x, b.y == y, b.z == z, b.w == w);
	}
	inline bool4 operator!=(bool4 b) noexcept {
		return bool4(b.x != x, b.y != y, b.z != z, b.w != w);
	}
	inline bool4 operator!() noexcept {
		return bool4(!x, !y, !z, !w);
	}
};

inline bool3::bool3(bool4 b) noexcept : x(b.x), y(b.y), z(b.z) {
}

inline void bool3::operator=(bool4 b) noexcept {
	x = b.x;
	y = b.y;
	z = b.z;
}

inline XMVECTOR XM_CALLCONV VectorGreater(
	const XMVECTOR& V1,
	const XMVECTOR& V2) noexcept {
#if defined(_XM_NO_INTRINSICS_)

	XMVECTORU32 Control = {{{(V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFF : 0}}};
	return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcgtq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpgt_ps(V1, V2);
#endif
}

inline XMVECTOR XM_CALLCONV VectorGreaterEqual(
	const XMVECTOR& V1,
	const XMVECTOR& V2) noexcept {
#if defined(_XM_NO_INTRINSICS_)

	XMVECTORU32 Control = {{{(V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0}}};
	return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcgeq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpge_ps(V1, V2);
#endif
}

inline XMVECTOR XM_CALLCONV VectorLess(
	const XMVECTOR& V1,
	const XMVECTOR& V2) noexcept {
#if defined(_XM_NO_INTRINSICS_)

	XMVECTORU32 Control = {{{(V1.vector4_f32[0] < V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[1] < V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[2] < V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[3] < V2.vector4_f32[3]) ? 0xFFFFFFFF : 0}}};
	return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcltq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmplt_ps(V1, V2);
#endif
}

inline XMVECTOR XM_CALLCONV VectorLessEqual(
	const XMVECTOR& V1,
	const XMVECTOR& V2) noexcept {
#if defined(_XM_NO_INTRINSICS_)

	XMVECTORU32 Control = {{{(V1.vector4_f32[0] <= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[1] <= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[2] <= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
							 (V1.vector4_f32[3] <= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0}}};
	return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcleq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmple_ps(V1, V2);
#endif
}

inline XMVECTOR XM_CALLCONV VectorEqual(
	const XMVECTOR& V1,
	const XMVECTOR& V2) noexcept {
#if defined(_XM_NO_INTRINSICS_)

	XMVECTORU32 Control = {{{
		(V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
		(V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
		(V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
		(V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFF : 0,
	}}};
	return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vceqq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpeq_ps(V1, V2);
#endif
}
INLINE bool3 GetBool3(const XMVECTOR& vec) noexcept {
	return bool3(
		vec.m128_u32[0] != 0,
		vec.m128_u32[1] != 0,
		vec.m128_u32[2] != 0);
}
INLINE bool4 GetBool4(const XMVECTOR& vec) noexcept {
	return bool4(
		vec.m128_u32[0] != 0,
		vec.m128_u32[1] != 0,
		vec.m128_u32[2] != 0,
		vec.m128_u32[3] != 0);
}
class Vector4;
class Vector3;
class Matrix3;
// A 3-vector with an unspecified fourth component.  Depending on the context, the W can be 0 or 1, but both are implicit.
// The actual value of the fourth component is undefined for performance reasons.

class Vector2 {
public:
	INLINE Vector2() noexcept {}
	INLINE Vector2(float x) noexcept : m_vec(x, x) {}
	INLINE Vector2(float x, float y) noexcept : m_vec(x, y) {}
	INLINE Vector2(const float2& v) noexcept : m_vec(v) {}
	INLINE float GetX() const noexcept { return m_vec.x; }
	INLINE float GetY() const noexcept { return m_vec.y; }
	INLINE void SetX(float x) noexcept { m_vec.x = x; }
	INLINE void SetY(float y) noexcept { m_vec.y = y; }

	INLINE Vector2 operator-() const noexcept { return Vector2(-m_vec.x, -m_vec.y); }
	INLINE Vector2 operator+(Vector2 v2) const noexcept { return Vector2(m_vec.x + v2.m_vec.x, m_vec.y + v2.m_vec.y); }
	INLINE Vector2 operator-(Vector2 v2) const noexcept { return Vector2(m_vec.x - v2.m_vec.x, m_vec.y - v2.m_vec.y); }
	INLINE Vector2 operator*(Vector2 v2) const noexcept { return Vector2(m_vec.x * v2.m_vec.x, m_vec.y * v2.m_vec.y); }
	INLINE Vector2 operator/(Vector2 v2) const noexcept { return Vector2(m_vec.x / v2.m_vec.x, m_vec.y / v2.m_vec.y); }
	INLINE Vector2 operator*=(Vector2 v2) const noexcept { return *this * Vector2(v2); }
	INLINE Vector2 operator/=(Vector2 v2) const noexcept { return *this / Vector2(v2); }
	INLINE Vector2 operator+=(Vector2 v2) const noexcept { return *this + Vector2(v2); }
	INLINE Vector2 operator-=(Vector2 v2) const noexcept { return *this - Vector2(v2); }

	INLINE bool operator<(Vector2 v2) {
		return v2.m_vec.x < m_vec.x && v2.m_vec.y < m_vec.y;
	}
	INLINE bool operator>(Vector2 v2) {
		return v2.m_vec.x > m_vec.x && v2.m_vec.y > m_vec.y;
	}
	INLINE bool operator==(Vector2 v2) {
		return v2.m_vec.x == m_vec.x && v2.m_vec.y == m_vec.y;
	}
	INLINE bool operator<=(Vector2 v2) {
		return v2.m_vec.x <= m_vec.x && v2.m_vec.y <= m_vec.y;
	}
	INLINE bool operator>=(Vector2 v2) {
		return v2.m_vec.x >= m_vec.x && v2.m_vec.y >= m_vec.y;
	}
	INLINE operator float4() const noexcept {
		return {m_vec.x, m_vec.y, 0, 0};
	}
	INLINE operator float3() const noexcept {
		return {m_vec.x, m_vec.y, 0};
	}
	INLINE operator float2() const noexcept {
		return m_vec;
	}
	INLINE friend Vector2 operator+(float v1, const Vector2& v2) noexcept { return v2.operator+(Vector2(v1)); }
	INLINE friend Vector2 operator-(float v1, const Vector2& v2) noexcept { return v2.operator-(Vector2(v1)); }
	INLINE friend Vector2 operator*(float v1, const Vector2& v2) noexcept { return v2.operator*(Vector2(v1)); }
	INLINE friend Vector2 operator/(float v1, const Vector2& v2) noexcept { return Vector2(v1 / v2.m_vec.x, v1 / v2.m_vec.y); }
	union {
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle2_1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		Swizzle<true, float, float, 0> x;
		Swizzle<true, float, float, 1> y;
		// ================================================color code================================================
		Swizzle<true, float, float, 0> r;
		Swizzle<true, float, float, 1> g;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle2_2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float2, 0, 0> xx;
		Swizzle<true, float, float2, 0, 1> xy;
		Swizzle<true, float, float2, 1, 0> yx;
		Swizzle<false, float, float2, 1, 1> yy;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle2_3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float3, 0, 0, 0> xxx;
		Swizzle<false, float, float3, 0, 0, 1> xxy;
		Swizzle<false, float, float3, 0, 1, 0> xyx;
		Swizzle<false, float, float3, 0, 1, 1> xyy;
		Swizzle<false, float, float3, 1, 0, 0> yxx;
		Swizzle<false, float, float3, 1, 0, 1> yxy;
		Swizzle<false, float, float3, 1, 1, 0> yyx;
		Swizzle<false, float, float3, 1, 1, 1> yyy;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle2_4!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float4, 0, 0, 0, 0> xxxx;
		Swizzle<false, float, float4, 0, 0, 0, 1> xxxy;
		Swizzle<false, float, float4, 0, 0, 1, 0> xxyx;
		Swizzle<false, float, float4, 0, 0, 1, 1> xxyy;
		Swizzle<false, float, float4, 0, 1, 0, 0> xyxx;
		Swizzle<false, float, float4, 0, 1, 0, 1> xyxy;
		Swizzle<false, float, float4, 0, 1, 1, 0> xyyx;
		Swizzle<false, float, float4, 0, 1, 1, 1> xyyy;
		Swizzle<false, float, float4, 1, 0, 0, 0> yxxx;
		Swizzle<false, float, float4, 1, 0, 0, 1> yxxy;
		Swizzle<false, float, float4, 1, 0, 1, 0> yxyx;
		Swizzle<false, float, float4, 1, 0, 1, 1> yxyy;
		Swizzle<false, float, float4, 1, 1, 0, 0> yyxx;
		Swizzle<false, float, float4, 1, 1, 0, 1> yyxy;
		Swizzle<false, float, float4, 1, 1, 1, 0> yyyx;
		Swizzle<false, float, float4, 1, 1, 1, 1> yyyy;
		float2 m_vec;
	};
};

class alignas(16) Vector3 {
	friend class Matrix3;
	friend class Vector4;

public:
	INLINE Vector3() noexcept {}
	INLINE Vector3(float x, float y, float z) noexcept : m_vec(XMVectorSet(x, y, z, 0)) {}
	INLINE Vector3(const float3& v) noexcept : m_vec(XMLoadFloat3(&v)) {}
	INLINE Vector3(const Vector3& v) noexcept : m_vec(v) {}
	INLINE Vector3(const Scalar& s) noexcept : m_vec(s) {
		m_vec.m128_f32[3] = 0;
	}
	INLINE Vector3(const Vector4& v) noexcept;
	INLINE void operator=(const Vector4& v) noexcept;
	INLINE Vector3(FXMVECTOR& vec) noexcept : m_vec(vec) {
		m_vec.m128_f32[3] = 0;
	}

	INLINE operator const XMVECTOR&() const noexcept { return m_vec; }

	INLINE float GetX() const noexcept { return m_vec.m128_f32[0]; }
	INLINE float GetY() const noexcept { return m_vec.m128_f32[1]; }
	INLINE float GetZ() const noexcept { return m_vec.m128_f32[2]; }
	INLINE void SetX(float x) noexcept { m_vec.m128_f32[0] = x; }
	INLINE void SetY(float y) noexcept { m_vec.m128_f32[1] = y; }
	INLINE void SetZ(float z) noexcept { m_vec.m128_f32[2] = z; }

	INLINE Vector3 operator-() const noexcept { return Vector3(XMVectorNegate(m_vec)); }
	INLINE Vector3 operator+(const Vector3& v2) const noexcept { return Vector3(XMVectorAdd(m_vec, v2)); }
	INLINE Vector3 operator-(const Vector3& v2) const noexcept { return Vector3(XMVectorSubtract(m_vec, v2)); }
	INLINE Vector3 operator+(const Scalar& v2) const noexcept { return Vector3(XMVectorAdd(m_vec, v2)); }
	INLINE Vector3 operator-(const Scalar& v2) const noexcept { return Vector3(XMVectorSubtract(m_vec, v2)); }
	INLINE Vector3 operator*(const Vector3& v2) const noexcept { return Vector3(XMVectorMultiply(m_vec, v2)); }
	INLINE Vector3 operator/(const Vector3& v2) const noexcept { return Vector3(XMVectorDivide(m_vec, v2)); }
	INLINE Vector3 operator*(const Scalar& v2) const noexcept { return *this * Vector3(v2); }
	INLINE Vector3 operator/(const Scalar& v2) const noexcept { return *this / Vector3(v2); }
	INLINE Vector3 operator*(float v2) const noexcept { return *this * Scalar(v2); }
	INLINE Vector3 operator/(float v2) const noexcept { return *this / Scalar(v2); }

	INLINE Vector3& operator+=(const Vector3& v) noexcept {
		*this = *this + v;
		return *this;
	}
	INLINE Vector3& operator+=(float v) noexcept {
		*this = *this + v;
		return *this;
	}
	INLINE Vector3& operator-=(const Vector3& v) noexcept {
		*this = *this - v;
		return *this;
	}
	INLINE Vector3& operator-=(float v) noexcept {
		*this = *this - v;
		return *this;
	}
	INLINE Vector3& operator*=(const Vector3& v) noexcept {
		*this = *this * v;
		return *this;
	}
	INLINE Vector3& operator*=(float v) noexcept {
		*this = *this * v;
		return *this;
	}
	INLINE Vector3& operator/=(const Vector3& v) noexcept {
		*this = *this / v;
		return *this;
	}
	INLINE Vector3& operator/=(float v) noexcept {
		*this = *this / v;
		return *this;
	}

	INLINE friend Vector3 operator*(const Scalar& v1, const Vector3& v2) noexcept { return Vector3(v1) * v2; }
	INLINE friend Vector3 operator/(const Scalar& v1, const Vector3& v2) noexcept { return Vector3(v1) / v2; }
	INLINE friend Vector3 operator*(float v1, const Vector3& v2) noexcept { return Scalar(v1) * v2; }
	INLINE friend Vector3 operator/(float v1, const Vector3& v2) noexcept { return Scalar(v1) / v2; }
	INLINE Vector3 operator<(const Vector3& a) const noexcept {
		return VectorLess(m_vec, a.m_vec);
	}
	INLINE Vector3 operator>(const Vector3& a) const noexcept {
		return VectorGreater(m_vec, a.m_vec);
	}
	INLINE Vector3 operator==(const Vector3& a) const noexcept {
		return VectorEqual(m_vec, a.m_vec);
	}

	INLINE Vector3 operator<=(const Vector3& a) const noexcept {
		return VectorLessEqual(m_vec, a.m_vec);
	}

	INLINE Vector3 operator>=(const Vector3& a) const noexcept {
		return VectorGreaterEqual(m_vec, a.m_vec);
	}
	INLINE operator float4() const noexcept {
		return {m_vec.m128_f32[0], m_vec.m128_f32[1], m_vec.m128_f32[2], 0};
	}
	INLINE operator float3() const noexcept {
		return {m_vec.m128_f32[0], m_vec.m128_f32[1], m_vec.m128_f32[2]};
	}
	INLINE float& operator[](uint index) noexcept {
		return m_vec.m128_f32[index];
	}
	INLINE float const& operator[](uint index) const noexcept {
		return m_vec.m128_f32[index];
	}
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle3_1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	union {
		Swizzle<true, float, float, 0> x;
		Swizzle<true, float, float, 1> y;
		Swizzle<true, float, float, 2> z;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle3_2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float2, 0, 0> xx;
		Swizzle<true, float, float2, 0, 1> xy;
		Swizzle<true, float, float2, 0, 2> xz;
		Swizzle<true, float, float2, 1, 0> yx;
		Swizzle<false, float, float2, 1, 1> yy;
		Swizzle<true, float, float2, 1, 2> yz;
		Swizzle<true, float, float2, 2, 0> zx;
		Swizzle<true, float, float2, 2, 1> zy;
		Swizzle<false, float, float2, 2, 2> zz;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle3_3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float3, 0, 0, 0> xxx;
		Swizzle<false, float, float3, 0, 0, 1> xxy;
		Swizzle<false, float, float3, 0, 0, 2> xxz;
		Swizzle<false, float, float3, 0, 1, 0> xyx;
		Swizzle<false, float, float3, 0, 1, 1> xyy;
		Swizzle<true, float, float3, 0, 1, 2> xyz;
		Swizzle<false, float, float3, 0, 2, 0> xzx;
		Swizzle<true, float, float3, 0, 2, 1> xzy;
		Swizzle<false, float, float3, 0, 2, 2> xzz;
		Swizzle<false, float, float3, 1, 0, 0> yxx;
		Swizzle<false, float, float3, 1, 0, 1> yxy;
		Swizzle<true, float, float3, 1, 0, 2> yxz;
		Swizzle<false, float, float3, 1, 1, 0> yyx;
		Swizzle<false, float, float3, 1, 1, 1> yyy;
		Swizzle<false, float, float3, 1, 1, 2> yyz;
		Swizzle<true, float, float3, 1, 2, 0> yzx;
		Swizzle<false, float, float3, 1, 2, 1> yzy;
		Swizzle<false, float, float3, 1, 2, 2> yzz;
		Swizzle<false, float, float3, 2, 0, 0> zxx;
		Swizzle<true, float, float3, 2, 0, 1> zxy;
		Swizzle<false, float, float3, 2, 0, 2> zxz;
		Swizzle<true, float, float3, 2, 1, 0> zyx;
		Swizzle<false, float, float3, 2, 1, 1> zyy;
		Swizzle<false, float, float3, 2, 1, 2> zyz;
		Swizzle<false, float, float3, 2, 2, 0> zzx;
		Swizzle<false, float, float3, 2, 2, 1> zzy;
		Swizzle<false, float, float3, 2, 2, 2> zzz;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle3_4!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float4, 0, 0, 0, 0> xxxx;
		Swizzle<false, float, float4, 0, 0, 0, 1> xxxy;
		Swizzle<false, float, float4, 0, 0, 0, 2> xxxz;
		Swizzle<false, float, float4, 0, 0, 1, 0> xxyx;
		Swizzle<false, float, float4, 0, 0, 1, 1> xxyy;
		Swizzle<false, float, float4, 0, 0, 1, 2> xxyz;
		Swizzle<false, float, float4, 0, 0, 2, 0> xxzx;
		Swizzle<false, float, float4, 0, 0, 2, 1> xxzy;
		Swizzle<false, float, float4, 0, 0, 2, 2> xxzz;
		Swizzle<false, float, float4, 0, 1, 0, 0> xyxx;
		Swizzle<false, float, float4, 0, 1, 0, 1> xyxy;
		Swizzle<false, float, float4, 0, 1, 0, 2> xyxz;
		Swizzle<false, float, float4, 0, 1, 1, 0> xyyx;
		Swizzle<false, float, float4, 0, 1, 1, 1> xyyy;
		Swizzle<false, float, float4, 0, 1, 1, 2> xyyz;
		Swizzle<false, float, float4, 0, 1, 2, 0> xyzx;
		Swizzle<false, float, float4, 0, 1, 2, 1> xyzy;
		Swizzle<false, float, float4, 0, 1, 2, 2> xyzz;
		Swizzle<false, float, float4, 0, 2, 0, 0> xzxx;
		Swizzle<false, float, float4, 0, 2, 0, 1> xzxy;
		Swizzle<false, float, float4, 0, 2, 0, 2> xzxz;
		Swizzle<false, float, float4, 0, 2, 1, 0> xzyx;
		Swizzle<false, float, float4, 0, 2, 1, 1> xzyy;
		Swizzle<false, float, float4, 0, 2, 1, 2> xzyz;
		Swizzle<false, float, float4, 0, 2, 2, 0> xzzx;
		Swizzle<false, float, float4, 0, 2, 2, 1> xzzy;
		Swizzle<false, float, float4, 0, 2, 2, 2> xzzz;
		Swizzle<false, float, float4, 1, 0, 0, 0> yxxx;
		Swizzle<false, float, float4, 1, 0, 0, 1> yxxy;
		Swizzle<false, float, float4, 1, 0, 0, 2> yxxz;
		Swizzle<false, float, float4, 1, 0, 1, 0> yxyx;
		Swizzle<false, float, float4, 1, 0, 1, 1> yxyy;
		Swizzle<false, float, float4, 1, 0, 1, 2> yxyz;
		Swizzle<false, float, float4, 1, 0, 2, 0> yxzx;
		Swizzle<false, float, float4, 1, 0, 2, 1> yxzy;
		Swizzle<false, float, float4, 1, 0, 2, 2> yxzz;
		Swizzle<false, float, float4, 1, 1, 0, 0> yyxx;
		Swizzle<false, float, float4, 1, 1, 0, 1> yyxy;
		Swizzle<false, float, float4, 1, 1, 0, 2> yyxz;
		Swizzle<false, float, float4, 1, 1, 1, 0> yyyx;
		Swizzle<false, float, float4, 1, 1, 1, 1> yyyy;
		Swizzle<false, float, float4, 1, 1, 1, 2> yyyz;
		Swizzle<false, float, float4, 1, 1, 2, 0> yyzx;
		Swizzle<false, float, float4, 1, 1, 2, 1> yyzy;
		Swizzle<false, float, float4, 1, 1, 2, 2> yyzz;
		Swizzle<false, float, float4, 1, 2, 0, 0> yzxx;
		Swizzle<false, float, float4, 1, 2, 0, 1> yzxy;
		Swizzle<false, float, float4, 1, 2, 0, 2> yzxz;
		Swizzle<false, float, float4, 1, 2, 1, 0> yzyx;
		Swizzle<false, float, float4, 1, 2, 1, 1> yzyy;
		Swizzle<false, float, float4, 1, 2, 1, 2> yzyz;
		Swizzle<false, float, float4, 1, 2, 2, 0> yzzx;
		Swizzle<false, float, float4, 1, 2, 2, 1> yzzy;
		Swizzle<false, float, float4, 1, 2, 2, 2> yzzz;
		Swizzle<false, float, float4, 2, 0, 0, 0> zxxx;
		Swizzle<false, float, float4, 2, 0, 0, 1> zxxy;
		Swizzle<false, float, float4, 2, 0, 0, 2> zxxz;
		Swizzle<false, float, float4, 2, 0, 1, 0> zxyx;
		Swizzle<false, float, float4, 2, 0, 1, 1> zxyy;
		Swizzle<false, float, float4, 2, 0, 1, 2> zxyz;
		Swizzle<false, float, float4, 2, 0, 2, 0> zxzx;
		Swizzle<false, float, float4, 2, 0, 2, 1> zxzy;
		Swizzle<false, float, float4, 2, 0, 2, 2> zxzz;
		Swizzle<false, float, float4, 2, 1, 0, 0> zyxx;
		Swizzle<false, float, float4, 2, 1, 0, 1> zyxy;
		Swizzle<false, float, float4, 2, 1, 0, 2> zyxz;
		Swizzle<false, float, float4, 2, 1, 1, 0> zyyx;
		Swizzle<false, float, float4, 2, 1, 1, 1> zyyy;
		Swizzle<false, float, float4, 2, 1, 1, 2> zyyz;
		Swizzle<false, float, float4, 2, 1, 2, 0> zyzx;
		Swizzle<false, float, float4, 2, 1, 2, 1> zyzy;
		Swizzle<false, float, float4, 2, 1, 2, 2> zyzz;
		Swizzle<false, float, float4, 2, 2, 0, 0> zzxx;
		Swizzle<false, float, float4, 2, 2, 0, 1> zzxy;
		Swizzle<false, float, float4, 2, 2, 0, 2> zzxz;
		Swizzle<false, float, float4, 2, 2, 1, 0> zzyx;
		Swizzle<false, float, float4, 2, 2, 1, 1> zzyy;
		Swizzle<false, float, float4, 2, 2, 1, 2> zzyz;
		Swizzle<false, float, float4, 2, 2, 2, 0> zzzx;
		Swizzle<false, float, float4, 2, 2, 2, 1> zzzy;
		Swizzle<false, float, float4, 2, 2, 2, 2> zzzz;
		XMVECTOR m_vec;
	};
};

// A 4-vector, completely defined.
class alignas(16) Vector4 {
public:
	friend class Vector3;
	INLINE Vector4() noexcept {}
	INLINE Vector4(float x, float y, float z, float w) noexcept : m_vec(XMVectorSet(x, y, z, w)) {}
	INLINE Vector4(const Vector3& xyz, float w) noexcept : m_vec(XMVectorSetW(xyz, w)) {}
	INLINE Vector4(const Vector4& v) noexcept : m_vec(v) {}
	INLINE Vector4(const Scalar& s) noexcept : m_vec(s) {}
	INLINE Vector4(const Vector3& xyz) noexcept : m_vec(SetWToOne(xyz)) {}
	INLINE void operator=(const Vector3& xyz) {
		m_vec = SetWToOne(xyz);
	}
	INLINE Vector4(const FXMVECTOR& vec) noexcept : m_vec(vec) {}
	INLINE Vector4(const float4& flt) noexcept : m_vec(XMLoadFloat4(&flt)) {}

	INLINE operator const XMVECTOR&() const noexcept { return m_vec; }

	INLINE float GetX() const noexcept { return m_vec.m128_f32[0]; }
	INLINE float GetY() const noexcept { return m_vec.m128_f32[1]; }
	INLINE float GetZ() const noexcept { return m_vec.m128_f32[2]; }
	INLINE float GetW() const noexcept { return m_vec.m128_f32[3]; }
	INLINE void SetX(float x) noexcept { m_vec.m128_f32[0] = x; }
	INLINE void SetY(float y) noexcept { m_vec.m128_f32[1] = y; }
	INLINE void SetZ(float z) noexcept { m_vec.m128_f32[2] = z; }
	INLINE void SetW(float w) noexcept { m_vec.m128_f32[3] = w; }

	INLINE Vector4 operator-() const noexcept { return Vector4(XMVectorNegate(m_vec)); }
	INLINE Vector4 operator+(const Vector4& v2) const noexcept { return Vector4(XMVectorAdd(m_vec, v2)); }
	INLINE Vector4 operator-(const Vector4& v2) const noexcept { return Vector4(XMVectorSubtract(m_vec, v2)); }
	INLINE Vector4 operator*(const Vector4& v2) const noexcept { return Vector4(XMVectorMultiply(m_vec, v2)); }
	INLINE Vector4 operator/(const Vector4& v2) const noexcept { return Vector4(XMVectorDivide(m_vec, v2)); }
	INLINE Vector4 operator*(const Scalar& v2) const noexcept { return *this * Vector4(v2); }
	INLINE Vector4 operator/(const Scalar& v2) const noexcept { return *this / Vector4(v2); }
	INLINE Vector4 operator+(const Scalar& v2) const noexcept { return *this + Vector4(v2); }
	INLINE Vector4 operator-(const Scalar& v2) const noexcept { return *this - Vector4(v2); }
	INLINE Vector4 operator*(float v2) const noexcept { return *this * Scalar(v2); }
	INLINE Vector4 operator/(float v2) const noexcept { return *this / Scalar(v2); }
	INLINE Vector4 operator+(float v2) const noexcept { return *this + Scalar(v2); }
	INLINE Vector4 operator-(float v2) const noexcept { return *this - Scalar(v2); }

	INLINE Vector4& operator*=(float v2) noexcept {
		*this = *this * Scalar(v2);
		return *this;
	}
	INLINE Vector4& operator/=(float v2) noexcept {
		*this = *this / Scalar(v2);
		return *this;
	}
	INLINE Vector4& operator+=(float v2) noexcept {
		*this = *this + Scalar(v2);
		return *this;
	}
	INLINE Vector4& operator-=(float v2) noexcept {
		*this = *this - Scalar(v2);
		return *this;
	}
	INLINE Vector4& operator+=(const Vector4& v) noexcept {
		*this = *this + v;
		return *this;
	}
	INLINE Vector4& operator-=(const Vector4& v) noexcept {
		*this = *this - v;
		return *this;
	}
	INLINE Vector4& operator*=(const Vector4& v) noexcept {
		*this = *this * v;
		return *this;
	}
	INLINE Vector4& operator/=(const Vector4& v) noexcept {
		*this = *this / v;
		return *this;
	}

	INLINE friend Vector4 operator*(const Scalar& v1, const Vector4& v2) noexcept { return Vector4(v1) * v2; }
	INLINE friend Vector4 operator/(const Scalar& v1, const Vector4& v2) noexcept { return Vector4(v1) / v2; }
	INLINE friend Vector4 operator*(float v1, const Vector4& v2) noexcept { return Scalar(v1) * v2; }
	INLINE friend Vector4 operator/(float v1, const Vector4& v2) noexcept { return Scalar(v1) / v2; }

	INLINE operator float4() const noexcept {
		return {m_vec.m128_f32[0], m_vec.m128_f32[1], m_vec.m128_f32[2], m_vec.m128_f32[3]};
	}
	INLINE operator float3() const noexcept {
		return {m_vec.m128_f32[0], m_vec.m128_f32[1], m_vec.m128_f32[2]};
	}

	INLINE Vector4 operator<(const Vector4& a) const noexcept {
		return VectorLess(m_vec, a.m_vec);
	}
	INLINE Vector4 operator>(const Vector4& a) const noexcept {
		return VectorGreater(m_vec, a.m_vec);
	}
	INLINE Vector4 operator==(const Vector4& a) const noexcept {
		return VectorEqual(m_vec, a.m_vec);
	}

	INLINE Vector4 operator<=(const Vector4& a) const noexcept {
		return VectorLessEqual(m_vec, a.m_vec);
	}

	INLINE Vector4 operator>=(const Vector4& a) const noexcept {
		return VectorGreaterEqual(m_vec, a.m_vec);
	}
	INLINE operator Vector3() const noexcept {
		return Vector3(m_vec);
	}
	INLINE float& operator[](uint index) noexcept {
		return m_vec.m128_f32[index];
	}
	INLINE float const& operator[](uint index) const noexcept {
		return m_vec.m128_f32[index];
	}
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle4_1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	union {
		Swizzle<true, float, float, 0> x;
		Swizzle<true, float, float, 1> y;
		Swizzle<true, float, float, 2> z;
		Swizzle<true, float, float, 3> w;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle4_2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float2, 0, 0> xx;
		Swizzle<true, float, float2, 0, 1> xy;
		Swizzle<true, float, float2, 0, 2> xz;
		Swizzle<true, float, float2, 0, 3> xw;
		Swizzle<true, float, float2, 1, 0> yx;
		Swizzle<false, float, float2, 1, 1> yy;
		Swizzle<true, float, float2, 1, 2> yz;
		Swizzle<true, float, float2, 1, 3> yw;
		Swizzle<true, float, float2, 2, 0> zx;
		Swizzle<true, float, float2, 2, 1> zy;
		Swizzle<false, float, float2, 2, 2> zz;
		Swizzle<true, float, float2, 2, 3> zw;
		Swizzle<true, float, float2, 3, 0> wx;
		Swizzle<true, float, float2, 3, 1> wy;
		Swizzle<true, float, float2, 3, 2> wz;
		Swizzle<false, float, float2, 3, 3> ww;
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle4_3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float3, 0, 0, 0> xxx;
		Swizzle<false, float, float3, 0, 0, 1> xxy;
		Swizzle<false, float, float3, 0, 0, 2> xxz;
		Swizzle<false, float, float3, 0, 0, 3> xxw;
		Swizzle<false, float, float3, 0, 1, 0> xyx;
		Swizzle<false, float, float3, 0, 1, 1> xyy;
		Swizzle<true, float, float3, 0, 1, 2> xyz;
		Swizzle<true, float, float3, 0, 1, 3> xyw;
		Swizzle<false, float, float3, 0, 2, 0> xzx;
		Swizzle<true, float, float3, 0, 2, 1> xzy;
		Swizzle<false, float, float3, 0, 2, 2> xzz;
		Swizzle<true, float, float3, 0, 2, 3> xzw;
		Swizzle<false, float, float3, 0, 3, 0> xwx;
		Swizzle<true, float, float3, 0, 3, 1> xwy;
		Swizzle<true, float, float3, 0, 3, 2> xwz;
		Swizzle<false, float, float3, 0, 3, 3> xww;
		Swizzle<false, float, float3, 1, 0, 0> yxx;
		Swizzle<false, float, float3, 1, 0, 1> yxy;
		Swizzle<true, float, float3, 1, 0, 2> yxz;
		Swizzle<true, float, float3, 1, 0, 3> yxw;
		Swizzle<false, float, float3, 1, 1, 0> yyx;
		Swizzle<false, float, float3, 1, 1, 1> yyy;
		Swizzle<false, float, float3, 1, 1, 2> yyz;
		Swizzle<false, float, float3, 1, 1, 3> yyw;
		Swizzle<true, float, float3, 1, 2, 0> yzx;
		Swizzle<false, float, float3, 1, 2, 1> yzy;
		Swizzle<false, float, float3, 1, 2, 2> yzz;
		Swizzle<true, float, float3, 1, 2, 3> yzw;
		Swizzle<true, float, float3, 1, 3, 0> ywx;
		Swizzle<false, float, float3, 1, 3, 1> ywy;
		Swizzle<true, float, float3, 1, 3, 2> ywz;
		Swizzle<false, float, float3, 1, 3, 3> yww;
		Swizzle<false, float, float3, 2, 0, 0> zxx;
		Swizzle<true, float, float3, 2, 0, 1> zxy;
		Swizzle<false, float, float3, 2, 0, 2> zxz;
		Swizzle<true, float, float3, 2, 0, 3> zxw;
		Swizzle<true, float, float3, 2, 1, 0> zyx;
		Swizzle<false, float, float3, 2, 1, 1> zyy;
		Swizzle<false, float, float3, 2, 1, 2> zyz;
		Swizzle<true, float, float3, 2, 1, 3> zyw;
		Swizzle<false, float, float3, 2, 2, 0> zzx;
		Swizzle<false, float, float3, 2, 2, 1> zzy;
		Swizzle<false, float, float3, 2, 2, 2> zzz;
		Swizzle<false, float, float3, 2, 2, 3> zzw;
		Swizzle<true, float, float3, 2, 3, 0> zwx;
		Swizzle<true, float, float3, 2, 3, 1> zwy;
		Swizzle<false, float, float3, 2, 3, 2> zwz;
		Swizzle<false, float, float3, 2, 3, 3> zww;
		Swizzle<false, float, float3, 3, 0, 0> wxx;
		Swizzle<true, float, float3, 3, 0, 1> wxy;
		Swizzle<true, float, float3, 3, 0, 2> wxz;
		Swizzle<false, float, float3, 3, 0, 3> wxw;
		Swizzle<true, float, float3, 3, 1, 0> wyx;
		Swizzle<false, float, float3, 3, 1, 1> wyy;
		Swizzle<true, float, float3, 3, 1, 2> wyz;
		Swizzle<false, float, float3, 3, 1, 3> wyw;
		Swizzle<true, float, float3, 3, 2, 0> wzx;
		Swizzle<true, float, float3, 3, 2, 1> wzy;
		Swizzle<false, float, float3, 3, 2, 2> wzz;
		Swizzle<false, float, float3, 3, 2, 3> wzw;
		Swizzle<false, float, float3, 3, 3, 0> wwx;
		Swizzle<false, float, float3, 3, 3, 1> wwy;
		Swizzle<false, float, float3, 3, 3, 2> wwz;
		Swizzle<false, float, float3, 3, 3, 3> www;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!swizzle4_4!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		Swizzle<false, float, float4, 0, 0, 0, 0> xxxx;
		Swizzle<false, float, float4, 0, 0, 0, 1> xxxy;
		Swizzle<false, float, float4, 0, 0, 0, 2> xxxz;
		Swizzle<false, float, float4, 0, 0, 0, 3> xxxw;
		Swizzle<false, float, float4, 0, 0, 1, 0> xxyx;
		Swizzle<false, float, float4, 0, 0, 1, 1> xxyy;
		Swizzle<false, float, float4, 0, 0, 1, 2> xxyz;
		Swizzle<false, float, float4, 0, 0, 1, 3> xxyw;
		Swizzle<false, float, float4, 0, 0, 2, 0> xxzx;
		Swizzle<false, float, float4, 0, 0, 2, 1> xxzy;
		Swizzle<false, float, float4, 0, 0, 2, 2> xxzz;
		Swizzle<false, float, float4, 0, 0, 2, 3> xxzw;
		Swizzle<false, float, float4, 0, 0, 3, 0> xxwx;
		Swizzle<false, float, float4, 0, 0, 3, 1> xxwy;
		Swizzle<false, float, float4, 0, 0, 3, 2> xxwz;
		Swizzle<false, float, float4, 0, 0, 3, 3> xxww;
		Swizzle<false, float, float4, 0, 1, 0, 0> xyxx;
		Swizzle<false, float, float4, 0, 1, 0, 1> xyxy;
		Swizzle<false, float, float4, 0, 1, 0, 2> xyxz;
		Swizzle<false, float, float4, 0, 1, 0, 3> xyxw;
		Swizzle<false, float, float4, 0, 1, 1, 0> xyyx;
		Swizzle<false, float, float4, 0, 1, 1, 1> xyyy;
		Swizzle<false, float, float4, 0, 1, 1, 2> xyyz;
		Swizzle<false, float, float4, 0, 1, 1, 3> xyyw;
		Swizzle<false, float, float4, 0, 1, 2, 0> xyzx;
		Swizzle<false, float, float4, 0, 1, 2, 1> xyzy;
		Swizzle<false, float, float4, 0, 1, 2, 2> xyzz;
		Swizzle<true, float, float4, 0, 1, 2, 3> xyzw;
		Swizzle<false, float, float4, 0, 1, 3, 0> xywx;
		Swizzle<false, float, float4, 0, 1, 3, 1> xywy;
		Swizzle<true, float, float4, 0, 1, 3, 2> xywz;
		Swizzle<false, float, float4, 0, 1, 3, 3> xyww;
		Swizzle<false, float, float4, 0, 2, 0, 0> xzxx;
		Swizzle<false, float, float4, 0, 2, 0, 1> xzxy;
		Swizzle<false, float, float4, 0, 2, 0, 2> xzxz;
		Swizzle<false, float, float4, 0, 2, 0, 3> xzxw;
		Swizzle<false, float, float4, 0, 2, 1, 0> xzyx;
		Swizzle<false, float, float4, 0, 2, 1, 1> xzyy;
		Swizzle<false, float, float4, 0, 2, 1, 2> xzyz;
		Swizzle<true, float, float4, 0, 2, 1, 3> xzyw;
		Swizzle<false, float, float4, 0, 2, 2, 0> xzzx;
		Swizzle<false, float, float4, 0, 2, 2, 1> xzzy;
		Swizzle<false, float, float4, 0, 2, 2, 2> xzzz;
		Swizzle<false, float, float4, 0, 2, 2, 3> xzzw;
		Swizzle<false, float, float4, 0, 2, 3, 0> xzwx;
		Swizzle<true, float, float4, 0, 2, 3, 1> xzwy;
		Swizzle<false, float, float4, 0, 2, 3, 2> xzwz;
		Swizzle<false, float, float4, 0, 2, 3, 3> xzww;
		Swizzle<false, float, float4, 0, 3, 0, 0> xwxx;
		Swizzle<false, float, float4, 0, 3, 0, 1> xwxy;
		Swizzle<false, float, float4, 0, 3, 0, 2> xwxz;
		Swizzle<false, float, float4, 0, 3, 0, 3> xwxw;
		Swizzle<false, float, float4, 0, 3, 1, 0> xwyx;
		Swizzle<false, float, float4, 0, 3, 1, 1> xwyy;
		Swizzle<true, float, float4, 0, 3, 1, 2> xwyz;
		Swizzle<false, float, float4, 0, 3, 1, 3> xwyw;
		Swizzle<false, float, float4, 0, 3, 2, 0> xwzx;
		Swizzle<true, float, float4, 0, 3, 2, 1> xwzy;
		Swizzle<false, float, float4, 0, 3, 2, 2> xwzz;
		Swizzle<false, float, float4, 0, 3, 2, 3> xwzw;
		Swizzle<false, float, float4, 0, 3, 3, 0> xwwx;
		Swizzle<false, float, float4, 0, 3, 3, 1> xwwy;
		Swizzle<false, float, float4, 0, 3, 3, 2> xwwz;
		Swizzle<false, float, float4, 0, 3, 3, 3> xwww;
		Swizzle<false, float, float4, 1, 0, 0, 0> yxxx;
		Swizzle<false, float, float4, 1, 0, 0, 1> yxxy;
		Swizzle<false, float, float4, 1, 0, 0, 2> yxxz;
		Swizzle<false, float, float4, 1, 0, 0, 3> yxxw;
		Swizzle<false, float, float4, 1, 0, 1, 0> yxyx;
		Swizzle<false, float, float4, 1, 0, 1, 1> yxyy;
		Swizzle<false, float, float4, 1, 0, 1, 2> yxyz;
		Swizzle<false, float, float4, 1, 0, 1, 3> yxyw;
		Swizzle<false, float, float4, 1, 0, 2, 0> yxzx;
		Swizzle<false, float, float4, 1, 0, 2, 1> yxzy;
		Swizzle<false, float, float4, 1, 0, 2, 2> yxzz;
		Swizzle<true, float, float4, 1, 0, 2, 3> yxzw;
		Swizzle<false, float, float4, 1, 0, 3, 0> yxwx;
		Swizzle<false, float, float4, 1, 0, 3, 1> yxwy;
		Swizzle<true, float, float4, 1, 0, 3, 2> yxwz;
		Swizzle<false, float, float4, 1, 0, 3, 3> yxww;
		Swizzle<false, float, float4, 1, 1, 0, 0> yyxx;
		Swizzle<false, float, float4, 1, 1, 0, 1> yyxy;
		Swizzle<false, float, float4, 1, 1, 0, 2> yyxz;
		Swizzle<false, float, float4, 1, 1, 0, 3> yyxw;
		Swizzle<false, float, float4, 1, 1, 1, 0> yyyx;
		Swizzle<false, float, float4, 1, 1, 1, 1> yyyy;
		Swizzle<false, float, float4, 1, 1, 1, 2> yyyz;
		Swizzle<false, float, float4, 1, 1, 1, 3> yyyw;
		Swizzle<false, float, float4, 1, 1, 2, 0> yyzx;
		Swizzle<false, float, float4, 1, 1, 2, 1> yyzy;
		Swizzle<false, float, float4, 1, 1, 2, 2> yyzz;
		Swizzle<false, float, float4, 1, 1, 2, 3> yyzw;
		Swizzle<false, float, float4, 1, 1, 3, 0> yywx;
		Swizzle<false, float, float4, 1, 1, 3, 1> yywy;
		Swizzle<false, float, float4, 1, 1, 3, 2> yywz;
		Swizzle<false, float, float4, 1, 1, 3, 3> yyww;
		Swizzle<false, float, float4, 1, 2, 0, 0> yzxx;
		Swizzle<false, float, float4, 1, 2, 0, 1> yzxy;
		Swizzle<false, float, float4, 1, 2, 0, 2> yzxz;
		Swizzle<true, float, float4, 1, 2, 0, 3> yzxw;
		Swizzle<false, float, float4, 1, 2, 1, 0> yzyx;
		Swizzle<false, float, float4, 1, 2, 1, 1> yzyy;
		Swizzle<false, float, float4, 1, 2, 1, 2> yzyz;
		Swizzle<false, float, float4, 1, 2, 1, 3> yzyw;
		Swizzle<false, float, float4, 1, 2, 2, 0> yzzx;
		Swizzle<false, float, float4, 1, 2, 2, 1> yzzy;
		Swizzle<false, float, float4, 1, 2, 2, 2> yzzz;
		Swizzle<false, float, float4, 1, 2, 2, 3> yzzw;
		Swizzle<true, float, float4, 1, 2, 3, 0> yzwx;
		Swizzle<false, float, float4, 1, 2, 3, 1> yzwy;
		Swizzle<false, float, float4, 1, 2, 3, 2> yzwz;
		Swizzle<false, float, float4, 1, 2, 3, 3> yzww;
		Swizzle<false, float, float4, 1, 3, 0, 0> ywxx;
		Swizzle<false, float, float4, 1, 3, 0, 1> ywxy;
		Swizzle<true, float, float4, 1, 3, 0, 2> ywxz;
		Swizzle<false, float, float4, 1, 3, 0, 3> ywxw;
		Swizzle<false, float, float4, 1, 3, 1, 0> ywyx;
		Swizzle<false, float, float4, 1, 3, 1, 1> ywyy;
		Swizzle<false, float, float4, 1, 3, 1, 2> ywyz;
		Swizzle<false, float, float4, 1, 3, 1, 3> ywyw;
		Swizzle<true, float, float4, 1, 3, 2, 0> ywzx;
		Swizzle<false, float, float4, 1, 3, 2, 1> ywzy;
		Swizzle<false, float, float4, 1, 3, 2, 2> ywzz;
		Swizzle<false, float, float4, 1, 3, 2, 3> ywzw;
		Swizzle<false, float, float4, 1, 3, 3, 0> ywwx;
		Swizzle<false, float, float4, 1, 3, 3, 1> ywwy;
		Swizzle<false, float, float4, 1, 3, 3, 2> ywwz;
		Swizzle<false, float, float4, 1, 3, 3, 3> ywww;
		Swizzle<false, float, float4, 2, 0, 0, 0> zxxx;
		Swizzle<false, float, float4, 2, 0, 0, 1> zxxy;
		Swizzle<false, float, float4, 2, 0, 0, 2> zxxz;
		Swizzle<false, float, float4, 2, 0, 0, 3> zxxw;
		Swizzle<false, float, float4, 2, 0, 1, 0> zxyx;
		Swizzle<false, float, float4, 2, 0, 1, 1> zxyy;
		Swizzle<false, float, float4, 2, 0, 1, 2> zxyz;
		Swizzle<true, float, float4, 2, 0, 1, 3> zxyw;
		Swizzle<false, float, float4, 2, 0, 2, 0> zxzx;
		Swizzle<false, float, float4, 2, 0, 2, 1> zxzy;
		Swizzle<false, float, float4, 2, 0, 2, 2> zxzz;
		Swizzle<false, float, float4, 2, 0, 2, 3> zxzw;
		Swizzle<false, float, float4, 2, 0, 3, 0> zxwx;
		Swizzle<true, float, float4, 2, 0, 3, 1> zxwy;
		Swizzle<false, float, float4, 2, 0, 3, 2> zxwz;
		Swizzle<false, float, float4, 2, 0, 3, 3> zxww;
		Swizzle<false, float, float4, 2, 1, 0, 0> zyxx;
		Swizzle<false, float, float4, 2, 1, 0, 1> zyxy;
		Swizzle<false, float, float4, 2, 1, 0, 2> zyxz;
		Swizzle<true, float, float4, 2, 1, 0, 3> zyxw;
		Swizzle<false, float, float4, 2, 1, 1, 0> zyyx;
		Swizzle<false, float, float4, 2, 1, 1, 1> zyyy;
		Swizzle<false, float, float4, 2, 1, 1, 2> zyyz;
		Swizzle<false, float, float4, 2, 1, 1, 3> zyyw;
		Swizzle<false, float, float4, 2, 1, 2, 0> zyzx;
		Swizzle<false, float, float4, 2, 1, 2, 1> zyzy;
		Swizzle<false, float, float4, 2, 1, 2, 2> zyzz;
		Swizzle<false, float, float4, 2, 1, 2, 3> zyzw;
		Swizzle<true, float, float4, 2, 1, 3, 0> zywx;
		Swizzle<false, float, float4, 2, 1, 3, 1> zywy;
		Swizzle<false, float, float4, 2, 1, 3, 2> zywz;
		Swizzle<false, float, float4, 2, 1, 3, 3> zyww;
		Swizzle<false, float, float4, 2, 2, 0, 0> zzxx;
		Swizzle<false, float, float4, 2, 2, 0, 1> zzxy;
		Swizzle<false, float, float4, 2, 2, 0, 2> zzxz;
		Swizzle<false, float, float4, 2, 2, 0, 3> zzxw;
		Swizzle<false, float, float4, 2, 2, 1, 0> zzyx;
		Swizzle<false, float, float4, 2, 2, 1, 1> zzyy;
		Swizzle<false, float, float4, 2, 2, 1, 2> zzyz;
		Swizzle<false, float, float4, 2, 2, 1, 3> zzyw;
		Swizzle<false, float, float4, 2, 2, 2, 0> zzzx;
		Swizzle<false, float, float4, 2, 2, 2, 1> zzzy;
		Swizzle<false, float, float4, 2, 2, 2, 2> zzzz;
		Swizzle<false, float, float4, 2, 2, 2, 3> zzzw;
		Swizzle<false, float, float4, 2, 2, 3, 0> zzwx;
		Swizzle<false, float, float4, 2, 2, 3, 1> zzwy;
		Swizzle<false, float, float4, 2, 2, 3, 2> zzwz;
		Swizzle<false, float, float4, 2, 2, 3, 3> zzww;
		Swizzle<false, float, float4, 2, 3, 0, 0> zwxx;
		Swizzle<true, float, float4, 2, 3, 0, 1> zwxy;
		Swizzle<false, float, float4, 2, 3, 0, 2> zwxz;
		Swizzle<false, float, float4, 2, 3, 0, 3> zwxw;
		Swizzle<true, float, float4, 2, 3, 1, 0> zwyx;
		Swizzle<false, float, float4, 2, 3, 1, 1> zwyy;
		Swizzle<false, float, float4, 2, 3, 1, 2> zwyz;
		Swizzle<false, float, float4, 2, 3, 1, 3> zwyw;
		Swizzle<false, float, float4, 2, 3, 2, 0> zwzx;
		Swizzle<false, float, float4, 2, 3, 2, 1> zwzy;
		Swizzle<false, float, float4, 2, 3, 2, 2> zwzz;
		Swizzle<false, float, float4, 2, 3, 2, 3> zwzw;
		Swizzle<false, float, float4, 2, 3, 3, 0> zwwx;
		Swizzle<false, float, float4, 2, 3, 3, 1> zwwy;
		Swizzle<false, float, float4, 2, 3, 3, 2> zwwz;
		Swizzle<false, float, float4, 2, 3, 3, 3> zwww;
		Swizzle<false, float, float4, 3, 0, 0, 0> wxxx;
		Swizzle<false, float, float4, 3, 0, 0, 1> wxxy;
		Swizzle<false, float, float4, 3, 0, 0, 2> wxxz;
		Swizzle<false, float, float4, 3, 0, 0, 3> wxxw;
		Swizzle<false, float, float4, 3, 0, 1, 0> wxyx;
		Swizzle<false, float, float4, 3, 0, 1, 1> wxyy;
		Swizzle<true, float, float4, 3, 0, 1, 2> wxyz;
		Swizzle<false, float, float4, 3, 0, 1, 3> wxyw;
		Swizzle<false, float, float4, 3, 0, 2, 0> wxzx;
		Swizzle<true, float, float4, 3, 0, 2, 1> wxzy;
		Swizzle<false, float, float4, 3, 0, 2, 2> wxzz;
		Swizzle<false, float, float4, 3, 0, 2, 3> wxzw;
		Swizzle<false, float, float4, 3, 0, 3, 0> wxwx;
		Swizzle<false, float, float4, 3, 0, 3, 1> wxwy;
		Swizzle<false, float, float4, 3, 0, 3, 2> wxwz;
		Swizzle<false, float, float4, 3, 0, 3, 3> wxww;
		Swizzle<false, float, float4, 3, 1, 0, 0> wyxx;
		Swizzle<false, float, float4, 3, 1, 0, 1> wyxy;
		Swizzle<true, float, float4, 3, 1, 0, 2> wyxz;
		Swizzle<false, float, float4, 3, 1, 0, 3> wyxw;
		Swizzle<false, float, float4, 3, 1, 1, 0> wyyx;
		Swizzle<false, float, float4, 3, 1, 1, 1> wyyy;
		Swizzle<false, float, float4, 3, 1, 1, 2> wyyz;
		Swizzle<false, float, float4, 3, 1, 1, 3> wyyw;
		Swizzle<true, float, float4, 3, 1, 2, 0> wyzx;
		Swizzle<false, float, float4, 3, 1, 2, 1> wyzy;
		Swizzle<false, float, float4, 3, 1, 2, 2> wyzz;
		Swizzle<false, float, float4, 3, 1, 2, 3> wyzw;
		Swizzle<false, float, float4, 3, 1, 3, 0> wywx;
		Swizzle<false, float, float4, 3, 1, 3, 1> wywy;
		Swizzle<false, float, float4, 3, 1, 3, 2> wywz;
		Swizzle<false, float, float4, 3, 1, 3, 3> wyww;
		Swizzle<false, float, float4, 3, 2, 0, 0> wzxx;
		Swizzle<true, float, float4, 3, 2, 0, 1> wzxy;
		Swizzle<false, float, float4, 3, 2, 0, 2> wzxz;
		Swizzle<false, float, float4, 3, 2, 0, 3> wzxw;
		Swizzle<true, float, float4, 3, 2, 1, 0> wzyx;
		Swizzle<false, float, float4, 3, 2, 1, 1> wzyy;
		Swizzle<false, float, float4, 3, 2, 1, 2> wzyz;
		Swizzle<false, float, float4, 3, 2, 1, 3> wzyw;
		Swizzle<false, float, float4, 3, 2, 2, 0> wzzx;
		Swizzle<false, float, float4, 3, 2, 2, 1> wzzy;
		Swizzle<false, float, float4, 3, 2, 2, 2> wzzz;
		Swizzle<false, float, float4, 3, 2, 2, 3> wzzw;
		Swizzle<false, float, float4, 3, 2, 3, 0> wzwx;
		Swizzle<false, float, float4, 3, 2, 3, 1> wzwy;
		Swizzle<false, float, float4, 3, 2, 3, 2> wzwz;
		Swizzle<false, float, float4, 3, 2, 3, 3> wzww;
		Swizzle<false, float, float4, 3, 3, 0, 0> wwxx;
		Swizzle<false, float, float4, 3, 3, 0, 1> wwxy;
		Swizzle<false, float, float4, 3, 3, 0, 2> wwxz;
		Swizzle<false, float, float4, 3, 3, 0, 3> wwxw;
		Swizzle<false, float, float4, 3, 3, 1, 0> wwyx;
		Swizzle<false, float, float4, 3, 3, 1, 1> wwyy;
		Swizzle<false, float, float4, 3, 3, 1, 2> wwyz;
		Swizzle<false, float, float4, 3, 3, 1, 3> wwyw;
		Swizzle<false, float, float4, 3, 3, 2, 0> wwzx;
		Swizzle<false, float, float4, 3, 3, 2, 1> wwzy;
		Swizzle<false, float, float4, 3, 3, 2, 2> wwzz;
		Swizzle<false, float, float4, 3, 3, 2, 3> wwzw;
		Swizzle<false, float, float4, 3, 3, 3, 0> wwwx;
		Swizzle<false, float, float4, 3, 3, 3, 1> wwwy;
		Swizzle<false, float, float4, 3, 3, 3, 2> wwwz;
		Swizzle<false, float, float4, 3, 3, 3, 3> wwww;
		XMVECTOR m_vec;
	};
};

INLINE Vector3::Vector3(const Vector4& v) noexcept {
	m_vec = SetWToZero(v);
}
INLINE void Vector3::operator=(const Vector4& v) noexcept {
	m_vec = SetWToZero(v);
}

}// namespace Math