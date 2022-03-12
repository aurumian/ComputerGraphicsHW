#pragma once

#include "Actor.h"

class CameraController : public Actor
{
public:

	void SetCameraToControl(class Camera* InCamera);
	virtual void Update(float DeltaTime) override;

	float Speed = 3.0f;
private:

	class Camera* Cam;
	
	float Pitch = 0.0f;
	float Yaw = 0.0f;

	const float MaxPitch = 85.0f;

	float FOV = 45.0f;

	float FOVChangeSpeed = 45.0f;

	float maxFov = 175.0f;
	float minFov = 45.0f; 

	float RotSpeedYaw = 100000.0f;
	float RotSpeedPitch = 30000.0f;
};