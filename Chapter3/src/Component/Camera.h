#pragma once
#include <stdafx.h>
#include <DXMath/DXMath.h>
#include <DXMath/MathHelper.h>
class Camera final {
public:
	~Camera();
	Camera();
	//  frustum properties.
	double NearZ() const { return mNearZ; }
	double FarZ() const { return mFarZ; }
	double Aspect() const { return mAspect; }
	double FovY() const { return mFovY; }
	double FovX() const {
		double halfWidth = 0.5 * NearWindowWidth();
		return 2.0 * atan(halfWidth / mNearZ);
	}

	//  near and far plane dimensions in view space coordinates.
	double NearWindowWidth() const { return mAspect * mNearWindowHeight; }
	double NearWindowHeight() const { return mNearWindowHeight; }
	double FarWindowWidth() const { return mAspect * mFarWindowHeight; }
	double FarWindowHeight() const { return mFarWindowHeight; }

	// Set frustum.
	void SetLens(double fovY, double aspect, double zn, double zf);

	// Define camera space via LookAt parameters.
	void LookAt(const Math::Vector3& pos, const Math::Vector3& target, const Math::Vector3& worldUp);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	Math::Matrix4 View = MathHelper::Identity4x4();
	Math::Matrix4 Proj = MathHelper::Identity4x4();
	Math::Vector3 Position = {0.0f, 0.0f, 0.0f};
	Math::Vector3 Right = {1.0f, 0.0f, 0.0f};
	Math::Vector3 Up = {0.0f, 1.0f, 0.0f};
	Math::Vector3 Forward = {0.0f, 0.0f, 1.0f};
	double orthoSize = 5;
	bool isOrtho = false;

private:
	double mNearZ = 0.0f;
	double mFarZ = 0.0f;
	double mAspect = 0.0f;
	double mFovY = 0.0f;
	double mNearWindowHeight = 0.0f;
	double mFarWindowHeight = 0.0f;
	
};
