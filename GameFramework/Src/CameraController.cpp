#include "CameraController.h"

#include "Camera.h"
#include "Game.h"
#include "InputDevice.h"

void CameraController::SetCameraToControl(Camera* InCamera)
{
	Cam = InCamera;
	FOV = Cam->GetFOV();
	minFov = FOV;
}

void CameraController::Update(float DeltaTime)
{
	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	Vector3 movementDelta = Vector3::Zero;

	if (input.IsKeyDown(87))
	{
		movementDelta.x += DeltaTime * Speed;
	}
	if (input.IsKeyDown(83))
	{
		movementDelta.x -= DeltaTime * Speed;
	}
	if (input.IsKeyDown(68))
	{
		movementDelta.y += DeltaTime * Speed;
	}
	if (input.IsKeyDown(65))
	{
		movementDelta.y -= DeltaTime * Speed;
	}
	if (input.IsKeyDown(69))
	{
		movementDelta.z += DeltaTime * Speed;
	}
	if (input.IsKeyDown(81))
	{
		movementDelta.z -= DeltaTime * Speed;
	}

	if (input.IsKeyDown(67))
	{
		FOV += DeltaTime * FOVChangeSpeed;
	}
	else
	{
		FOV -= DeltaTime * FOVChangeSpeed;
	}

	FOV = FOV < minFov ? minFov : (FOV > maxFov ? maxFov : FOV);

	Cam->UpdateFOV(FOV);

	Cam->Transform.Position += Cam->Transform.Rotation.GetForwardVector() * movementDelta.x
		+ Cam->Transform.Rotation.GetRightVector() * movementDelta.y
		+ Vector3::Up * movementDelta.z;

	float deltaX;
	float deltaY;
	input.GetMouseDelta(deltaX, deltaY);

	Pitch += DeltaTime * deltaY * RotSpeedPitch;
	Pitch = Pitch < -MaxPitch ? -MaxPitch : Pitch > MaxPitch ? MaxPitch : Pitch;
	Yaw += DeltaTime * deltaX * RotSpeedYaw;

	Cam->Transform.Rotation.SetEulerAngles(Pitch, Yaw, 0.0f);
}
