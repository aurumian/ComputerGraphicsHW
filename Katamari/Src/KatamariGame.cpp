#include "KatamariGame.h"

#include "Mesh.h"
#include "ShaderCompiler.h"
#include "DisplayWin32.h"
#include "OrbitCameraController.h"
#include "MeshRenderer.h"
#include "InputDevice.h"
#include "PlaneComponent.h"

#include <iostream>
using namespace std;

void KatamariGame::PrepareResources()
{
	Game::PrepareResources();

	// create meshes
	Mesh* sphereMesh = new SphereMesh();


	// create shaders
	ShaderCompiler sc;
	sc.SetIsDebug(true);
	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	sc.SetPathToShader(L"../../Shaders/MyVeryFirstShader.hlsl");

	SimpleVertexShader* vs = sc.CreateShader<SimpleVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	PixelShader* ps = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("PSPlainColor");
	PixelShader* psPlain = sc.CreateShader<PixelShader>();



	// Setup PerspCamera
	CurrentCamera = new Camera();
	CurrentCamera->UpdateProjectionMatrixPerspective(45.0f, Display->GetAspectRatio(), 0.01f, 1000.0f);
	CurrentCamera->Transform.Position = CurrentCamera->Transform.Rotation.GetForwardVector() * -5.0f;

	OrbitCameraController* orbitCC = CreateGameComponent<OrbitCameraController>();
	orbitCC->SetCameraToControl(CurrentCamera);
	
	Player = CreateGameComponent<GameComponent>();
	orbitCC->GCToOrbit = Player;

	MeshRenderer* playerMesh = Player->AddChildComponent<MeshRenderer>();
	playerMesh->SetPixelShader(ps);
	playerMesh->SetVertexShader(vs);
	playerMesh->SetMesh(sphereMesh);

	MeshRenderer* mr = playerMesh->AddChildComponent<MeshRenderer>();
	mr->SetPixelShader(psPlain);
	mr->SetColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	mr->SetVertexShader(vs);
	mr->SetMesh(sphereMesh);
	mr->mTransform.Position.y = 1.0f;
	mr->mTransform.Scale = Vector3::One * 0.2f;

	mr = playerMesh->AddChildComponent<MeshRenderer>();
	mr->SetPixelShader(psPlain);
	mr->SetColor(Color(0.0f, 0.0f, 1.0f, 1.0f));
	mr->SetVertexShader(vs);
	mr->SetMesh(sphereMesh);
	mr->mTransform.Position.z = -1.0f;
	mr->mTransform.Scale = Vector3::One * 0.2f;

	PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	pc->SetPixelShader(ps);
	pc->SetVertexShader(vs);
	
}

void KatamariGame::Update(float DeltaTime)
{
	Game::Update(DeltaTime);
	InputDevice& input = *GetInputDevice();
	Vector3 movementDelta;
	float Speed = 5.0f;
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
	const float radius = 1.0f;
	const float rotAngleX = -movementDelta.x / radius;
	const float rotAngleY = -movementDelta.y / radius;
	const Vector3 right = CurrentCamera->Transform.Rotation.GetRightVector();
	Player->mTransform.Rotation.RotateAroundAxis(right, rotAngleX);
	const Vector3 forward = right.Cross(Vector3::Up);
	Player->mTransform.Rotation.RotateAroundAxis(forward, rotAngleY);
	Player->mTransform.Position -= forward * movementDelta.x;
	Player->mTransform.Position += right * movementDelta.y;

	
}
