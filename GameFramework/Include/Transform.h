#pragma once

#include "MathInclude.h"

#include <DirectXMath.h>
#include <string>

struct Rotator
{
protected:
	Quaternion Quat;

public:

	Rotator();
	Rotator(Vector3 EulerAngles);
	Rotator(Quaternion InQuat);

	// Set rotation with euler angles in degrees
	void SetEulerAngles(Vector3 EulerAngles);
	void SetEulerAngles(float X, float Y, float Z);

	Vector3 GetEulerDegrees() const;

	Vector3 GetEulerRadians() const;

	Vector3 GetForwardVector() const;

	Vector3 GetUpVector() const;

	Vector3 GetRightVector() const;

	//// rotates angle degrees around given axis
	//void RotateArounAxis(float angle, const DirectX::XMFLOAT3 axis);

	Quaternion GetQuaterion() const;

	friend struct Transform;
};

struct Transform
{
	Vector3 Position = Vector3::Zero;
	Rotator Rotation;
	Vector3 Scale = Vector3::One;

	Matrix GetTransformMatrix() const;

	Matrix GetTransformMatrixTransposed() const;

	Matrix GetInverseTransformMatrix() const;

	Matrix GetInverseTransformMatrixTransposed() const;

	bool SetFromMatrix(Matrix Mat);

	bool SetFromMatrixIgnoreScale(Matrix Mat);


	//DirectX::XMFLOAT3X3 GetNormalMatrix() const;

	//std::string ToString() const;
};

