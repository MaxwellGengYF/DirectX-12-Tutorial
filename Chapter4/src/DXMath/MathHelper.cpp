#include <DXMath/MathHelper.h>
#include <float.h>
#include <cmath>
using namespace DirectX;
const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;
float MathHelper::AngleFromXY(float x, float y) {
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f) {
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x);// in [-pi/2, +pi/2]
		if (theta < 0.0f)
			theta += 2.0f * Pi;// in [0, 2*pi).
	}
	// Quadrant II or III
	else
		theta = atanf(y / x) + Pi;// in [0, 2*pi).
	return theta;
}
XMVECTOR MathHelper::RandUnitVec3() {
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();
	// Keep trying until we get a point on/in the hemisphere.
	while (true) {
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);
		// Ignore points outside the unit sphere in order to get an even distribution
		// over the unit sphere.  Otherwise points will clump more on the sphere near
		// the corners of the cube.
		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;
		return XMVector3Normalize(v);
	}
}
XMVECTOR MathHelper::RandHemisphereUnitVec3(XMVECTOR n) {
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();
	// Keep trying until we get a point on/in the hemisphere.
	while (true) {
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);
		// Ignore points outside the unit sphere in order to get an even distribution
		// over the unit sphere.  Otherwise points will clump more on the sphere near
		// the corners of the cube.

		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;
		// Ignore points in the bottom hemisphere.
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;
		return XMVector3Normalize(v);
	}
}

DirectX::XMVECTOR MathHelper::SphericalToCartesian(float radius, float theta, float phi) {
	return DirectX::XMVectorSet(
		radius * sinf(phi) * cosf(theta),
		radius * cosf(phi),
		radius * sinf(phi) * sinf(theta),
		1.0f);
}

DirectX::XMMATRIX MathHelper::InverseTranspose(DirectX::CXMMATRIX M) {
	// Inverse-transpose is just applied to normals.  So zero out
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
}

float4x4 MathHelper::Identity4x4() {
	static float4x4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}