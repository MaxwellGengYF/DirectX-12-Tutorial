#include <Component/Camera.h>
using namespace DirectX;
using namespace Math;
Camera::Camera() {
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1000.0f);
}
Camera::~Camera() {
}
void Camera::SetLens(float fovY, float zn, float zf) {
	// cache properties
	mFovY = fovY;
	mNearZ = zn;
	mFarZ = zf;
	mNearWindowHeight = 2.0f * mNearZ * tan(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tan(0.5f * mFovY);
}
void Camera::SetAspect(float aspect) {
	mAspect = aspect;
}

void Camera::LookAt(const Math::Vector3& pos, const Math::Vector3& target, const Math::Vector3& worldUp) {
	Forward = normalize(target - pos);
	Right = normalize(cross(worldUp, Forward));
	Up = cross(Forward, Right);
	Position = pos;
}
void Camera::UpdateProjectionMatrix() {
	if (isOrtho) {
		mFarZ = Max(mFarZ, mNearZ + 0.1f);
		Matrix4 P = XMMatrixOrthographicLH(orthoSize, orthoSize, mNearZ, mFarZ);
		Proj = P;
	} else {
		mNearZ = Max(mNearZ, 0.01f);
		mFarZ = Max(mFarZ, mNearZ + 0.1f);
		Matrix4 P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
		Proj = P;
	}
}
void Camera::UpdateViewMatrix() {
	View = GetInverseTransformMatrix(Right, Up, Forward, Position);
}
