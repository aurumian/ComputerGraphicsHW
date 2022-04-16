#include "KatamariGame.h"

#include "Mesh.h"
#include "ShaderCompiler.h"
#include "DisplayWin32.h"
#include "OrbitCameraController.h"
#include "MeshRenderer.h"
#include "InputDevice.h"
#include "PlaneComponent.h"
#include "SphereCollider.h"
#include "LineRenderer.h"

#include "MeshLoader.h"
#include "MeshProxy.h"

#include "WICTextureLoader.h"

#include <iostream>
using namespace std;

void KatamariGame::PrepareResources()
{
	Game::PrepareResources();

#pragma region Create Meshes
	SphereMesh sphereMesh = SphereMesh();
	MeshProxy* sphereMeshProxy = sphereMesh.CreateMeshProxy();

	TexturedSquareMesh squareMesh;
	MeshProxy* squareMeshProxy = squareMesh.CreateMeshProxy();

	MeshLoader ml = MeshLoader("C:\\Users\\titan\\Desktop\\WorldClock\\Models\\FBX\\staff.fbx");
	TexturedMesh staff = ml.GetMesh(0);
	MeshProxy* staffMeshProxy = staff.CreateMeshProxy();

	ml.OpenFile("C:\\Users\\titan\\Downloads\\32-coffee_cup_fbx\\coffee_cup_fbx.fbx");
	TexturedMesh cup = ml.GetMesh(0);
	MeshProxy* cupMeshProxy = cup.CreateMeshProxy();

	ml.OpenFile("C:\\Users\\titan\\Downloads\\PepsiCan\\PepsiCan.fbx");
	TexturedMesh canMesh = ml.GetMesh(0);
	canMeshProxy = canMesh.CreateMeshProxy();

	ml.OpenFile("C:\\Users\\titan\\Downloads\\PizzaBox\\PizzaBox.obj");
	TexturedMesh pizzaBoxMesh = ml.GetMesh(0);
	MeshProxy* pizzaBoxMeshProxy = pizzaBoxMesh.CreateMeshProxy();

#pragma endregion Create Meshes

#pragma region Create Shaders
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

	sc.SetPathToShader(L"../../Shaders/BasicShader.hlsl");
	sc.SetEntryPoint("PSMain");
	basicPS = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	basicVS = sc.CreateShader<BasicVertexShader>();

	sc.SetPathToShader(L"../../Shaders/TexturedShader.hlsl");
	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");
	texturedPS = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	texturedVS = sc.CreateShader<TexturedVertexShader>();
#pragma endregion Create Shaders

	// todo: encapsulate texture?
	// Load textures
	CreateWICTextureFromFile(GetD3DDevice().Get(), L"C:\\Users\\titan\\Downloads\\31-textures\\textures\\Base_color.png", cupTexResource.GetAddressOf(), cupTexSRV.GetAddressOf(), 1024);
	CreateWICTextureFromFile(GetD3DDevice().Get(), L"C:\\Users\\titan\\Downloads\\PepsiCan\\PepsiCan.png", pepsiTexResource.GetAddressOf(), pepsiTexSRV.GetAddressOf(), 2048);
	CreateWICTextureFromFile(GetD3DDevice().Get(), L"C:\\Users\\titan\\Downloads\\PizzaBox\\PizzaBox.jpg", pizzaBoxTexResource.GetAddressOf(), pizzaBoxTexSRV.GetAddressOf(), 2048);

	// Setup PerspCamera
	CurrentCamera = new Camera();
	CurrentCamera->UpdateProjectionMatrixPerspective(45.0f, Display->GetAspectRatio(), 0.01f, 1000.0f);
	CurrentCamera->Transform.Position = CurrentCamera->Transform.Rotation.GetForwardVector() * -5.0f;

	OrbitCameraController* orbitCC = CreateGameComponent<OrbitCameraController>();
	orbitCC->SetCameraToControl(CurrentCamera);
	orbitCC->OrbitRadius = 10.0f;
	
	Player = CreateGameComponent<KatamariComponent>();
	Player->InitLineSphere(basicPS, basicVS, Color(1.0f, 1.0f, 0.0f));
	Player->SetRadius(0.52f);
	orbitCC->GCToOrbit = Player;
	MeshRenderer* playerMesh = Player->AddChildComponent<MeshRenderer>();
	playerMesh->SetPixelShader(ps);
	playerMesh->SetVertexShader(vs);
	playerMesh->SetMeshProxy(sphereMeshProxy);
	playerMesh->mTransform.Scale = Vector3::One * 0.5f;

	MeshRenderer* mr = nullptr;
	MeshRenderer* staff1 = CreateGameComponent<MeshRenderer>();
	staff1->SetMeshProxy(staffMeshProxy);
	staff1->SetPixelShader(ps);
	staff1->SetVertexShader(vs);

	KatamariComponent* kcCup = CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(3.0f, 0.0f, 0.0f), 0.26f, LitMaterial(), Vector3::One, Vector3(0.0f, -0.3f, 0.0f));
	kcCup = CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(4.0f, 0.0f, 0.0f), 0.26f, LitMaterial(), Vector3::One, Vector3(0.0f, -0.3f, 0.0f));
	kcCup->mTransform.Rotation = Vector3(0.0f, 0.0f, 85.0f);
	kcCup = CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(3.0f, 0.0f, -1.0f), 0.26f, LitMaterial(), Vector3::One, Vector3(0.0f, -0.3f, 0.0f));
	kcCup->mTransform.Rotation = Vector3(0.0f, 40.0f, -85.0f);
	CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(4.0f, 0.0f, -2.0f), 0.26f, LitMaterial(), Vector3::One, Vector3(0.0f, -0.3f, 0.0f));
	kcCup = CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(5.2f, 0.0f, -1.0f), 0.65f, LitMaterial(), Vector3::One * 2, Vector3(0.0f, -0.5f, 0.0f));
	kcCup->mTransform.Rotation = Vector3(0.0f, 50.0f, -85.0f);
	kcCup = CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(7.7f, 0.0f, -2.0f), 0.65f, LitMaterial(), Vector3::One * 2, Vector3(0.0f, -0.5f, 0.0f));
	kcCup->mTransform.Rotation = Vector3(0.0f, 40.0f, 85.0f);
	kcCup = CreateKatamariComponent(cupMeshProxy, cupTexSRV, Vector3(4.1f, 0.0f, -4.5f), 0.65f, LitMaterial(), Vector3::One * 2, Vector3(0.0f, -0.5f, 0.0f));
	kcCup->mTransform.Rotation = Vector3(0.0f, -52.0f, 85.0f);

	MeshRenderer* texPlane = CreateGameComponent<MeshRenderer>();
	texPlane->bCastShadow = false;
	texPlane->SetMeshProxy(squareMeshProxy);
	texPlane->SetPixelShader(texturedPS);
	texPlane->SetVertexShader(texturedVS);
	texPlane->SetAlbedoSRV(ShadowMapSRV);
	texPlane->mTransform.Position = Vector3(0.0f, 3.0f, 1.0f);

	//KatamariComponent* can1 = CreateKatamariComponent(canMeshProxy, pepsiTexSRV, Vector3(-3.0f, 0.0f, 0.0f), 0.26f, Vector3::One * 0.2f, Vector3(0.0f, -0.5f, 0.0f));
	CreateCanComponent(Vector3(-3.0f, 0.0f, 0.0f), 0.26f, Vector3::One * 0.2f);
	CreateCanComponent(Vector3(-3.0f, 0.0f, 1.0f), 0.26f, Vector3::One * 0.2f);
	CreateCanComponent(Vector3(-2.0f, 0.0f, 1.0f), 0.26f, Vector3::One * 0.2f);
	CreateCanComponent(Vector3(-3.0f, 0.0f, 3.0f), 0.26f, Vector3::One * 0.2f);
	CreateCanComponent(Vector3(-4.0f, 0.0f, 4.0f), 0.65f, Vector3(0.5f, 0.5f, 0.25f));
	CreateCanComponent(Vector3(-5.2f, 0.0f, -2.0f), 0.65f, Vector3(0.5f, 0.5f, 0.25f));
	CreateCanComponent(Vector3(-4.4f, 0.0f, -4.0f), 0.65f, Vector3(0.5f, 0.5f, 0.25f));
	
	KatamariComponent* pizzaBox1 = CreateKatamariComponent(pizzaBoxMeshProxy, pizzaBoxTexSRV, Vector3(0.0f, 0.0f, -3.0f), 1.1f, LitMaterial(), Vector3::One * 0.04f);
	

	PlaneComponent* pc = CreateGameComponent<PlaneComponent>();
	pc->SetPixelShader(basicPS);
	pc->SetVertexShader(basicVS);
	pc->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));

	// Setup Light
	// todo: Create a DirectionalLightComponent and get DirLight from it - this will make rotating a bit easier
	LightCam.Transform.Rotation = Vector3(-70.0f, 0.0f, 0.0f);
	LightCam.Transform.Position = Vector3(0.0f, 5.0f, 0.0f);
	LightCam.UpdateProjectionMatrixOrthographic(20.0f, 20.0f, 0.0f, 100.0f);
	DirectionalLight.direction = LightCam.Transform.Rotation.GetForwardVector();

	MeshRenderer* floor = CreateGameComponent<MeshRenderer>();
	floor->SetMeshProxy(squareMeshProxy);
	floor->SetPixelShader(texturedPS);
	floor->SetVertexShader(texturedVS);
	floor->SetAlbedoSRV(pizzaBoxTexSRV);
	floor->mTransform.Position = Vector3(0.0f, -1.0f, 0.0f);
	floor->mTransform.Rotation = Vector3(-90.0f, 0.0f, 0.0f);
	floor->mTransform.Scale = Vector3::One * 10.0f;
}

void KatamariGame::Update(float DeltaTime)
{
	Game::Update(DeltaTime);

#pragma region Player Movement
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
#pragma endregion Player Movement

#pragma region Object Capture
	Collider* col = GetOverlapping(Player->GetSphereCollider());
	if (col != nullptr)
	{
		KatamariComponent* kc = col->GetComponentInParent<KatamariComponent>();
		if (kc != nullptr)
		{
			if (kc->GetRadius() <= Player->GetRadius())
			{
				// Attach the object to player
				kc->mTransform = kc->GetWorldTransform().GetTransformMatrix() * Player->mTransform.GetInverseTransformMatrix();
				kc->SetParent(Player);
				const float pr = Player->GetRadius();
				const float kcr = kc->GetRadius();
				kc->DestroyShpereCollider();

				// Update player radius as if the player's sphere volume was increased by volume of the captured object
				Player->SetRadius(powf(pr * pr * pr + kcr * kcr * kcr, 1.0f / 3.0f));
			}
		}
	}
	
#pragma endregion Object Capture
	
}

KatamariComponent* KatamariGame::CreateKatamariComponent(MeshProxy* InMesh, 
	ComPtr<ID3D11ShaderResourceView> TexSRV, 
	Vector3 Position, 
	float Radius, 
	const LitMaterial& mat,
	Vector3 MeshScale, 
	Vector3 MeshLocalPos)
{
	KatamariComponent* kc = CreateGameComponent<KatamariComponent>();
	kc->InitLineSphere(basicPS, basicVS, Color(0.0f, 1.0f, 1.0f, 1.0f));
	kc->SetRadius(Radius);
	MeshRenderer* mr = kc->AddChildComponent<MeshRenderer>();
	mr->SetMeshProxy(InMesh);
	mr->SetPixelShader(texturedPS);
	mr->SetVertexShader(texturedVS);
	mr->SetAlbedoSRV(TexSRV);
	mr->mTransform.Rotation = Vector3(-90.0f, 0.0f, 0.0f);
	mr->mTransform.Scale = MeshScale;
	mr->mTransform.Position = MeshLocalPos;
	mr->Mat = mat;
	kc->mTransform.Position = Position;
	return kc;
}

KatamariComponent* KatamariGame::CreateCanComponent(Vector3 Position, float Radius, Vector3 MeshScale)
{
	static LitMaterial canMat;
	canMat.ambientCoef = 0.232125f;
	canMat.diffuesCoef = 0.7;
	canMat.specularCoef = 0.773911;
	canMat.specularExponent = 30;
	return CreateKatamariComponent(canMeshProxy, pepsiTexSRV, Position, Radius, canMat, MeshScale, Vector3(0.0f, -0.5f, 0.0f) * MeshScale.z * 5.0f);
}

KatamariComponent::KatamariComponent()
{
	mSphereCollider = AddChildComponent<SphereCollider>();
	mLineSphere = AddChildComponent<LineSphere>();
	
	SetRadius(mRadius);
}

void KatamariComponent::SetRadius(float NewRadius)
{
	mRadius = NewRadius;

	if (mSphereCollider)
	{
		mSphereCollider->Radius = mRadius;
	}

	if (mLineSphere)
	{
		mLineSphere->mTransform.Scale = Vector3::One * mRadius;
	}
}

void KatamariComponent::InitLineSphere(PixelShader* InPixelShader, VertexShader* InVertexShader, Color InColor)
{
	mLineSphere->SetPixelShader(InPixelShader);
	mLineSphere->SetVertexShader(InVertexShader);
	mLineSphere->SetColor(InColor);
}

void KatamariComponent::DestroyShpereCollider()
{
	Game::GetInstance()->DestroyComponent(mSphereCollider);
	mSphereCollider = nullptr;
}
