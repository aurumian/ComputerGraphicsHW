#pragma once

#include "Game.h"

#include "ActorComponent.h"
#include "Actor.h"

class RotateAroundActorComponent : public ActorComponent
{
public:
	Actor* ActorToRotateAround = nullptr;

	float OrbitRadius = 2.0f;

	float RotAngle = 0.0f;

	// radians per second
	float RotSpeed = 0.6f;

	Vector3 Scale = Vector3::One;

	virtual void Update(float DeltaTime) override
	{
		if (ActorToRotateAround != nullptr)
		{
			RotAngle += DeltaTime * RotSpeed;
			if (RotAngle > 6.28f)
			{
				RotAngle -= 6.28f;
			}

			const Transform parentTransform = ActorToRotateAround->Transform;
			Matrix mat = Matrix::CreateScale(Scale) * Matrix::CreateTranslation(Vector3(OrbitRadius, 0.0f, 0.0f)) *  Matrix::CreateFromAxisAngle(Vector3::Up, RotAngle) * parentTransform.GetTransformMatrix();
			this->GetOwner()->Transform.SetFromMatrix(mat);

		}
	}
};

class HW2Game : public Game
{
public:
	virtual void PrepareResources() override;

private:
	class BoxMesh* boxMesh;
	class CircleMesh* circleMesh;
	class SphereMesh* sphereMesh;
	class SimpleVertexShader* vs;
	class PixelShader* ps;
	class PixelShader* psPlain;
};

