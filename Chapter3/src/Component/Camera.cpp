#include <Component/Camera.h>
using namespace DirectX;
using namespace Math;
Camera::Camera() {
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}
Camera::~Camera() {
}
void Camera::SetLens(double fovY, double aspect, double zn, double zf) {
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;
	mNearWindowHeight = 2.0 * (double)mNearZ * tan(0.5 * mFovY);
	mFarWindowHeight = 2.0 * (double)mFarZ * tan(0.5 * mFovY);
}
void Camera::LookAt(const Math::Vector3& pos, const Math::Vector3& target, const Math::Vector3& worldUp) {
	Forward = normalize(target - pos);
	Right = normalize(cross(worldUp, Forward));
	Up = cross(Forward, Right);
	Position = pos;
}
void Camera::UpdateProjectionMatrix() {
	// Reverse Z
	if (isOrtho) {
		mFarZ = Max(mFarZ, mNearZ + 0.1);
		Matrix4 P = XMMatrixOrthographicLH(orthoSize, orthoSize, mFarZ, mNearZ);
		Proj = P;
	} else {
		mNearZ = Max(mNearZ, 0.01);
		mFarZ = Max(mFarZ, mNearZ + 0.1);
		Matrix4 P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mFarZ, mNearZ);
		Proj = P;
	}
}
void Camera::UpdateViewMatrix() {
	View = GetInverseTransformMatrix(Right, Up, Forward, Position);
}
