#include "HW2Game.h"

#include "ShaderCompiler.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "DisplayWin32.h"
#include "MeshRenderer.h"
#include "CameraController.h"

void HW2Game::PrepareResources()
{
	Game::PrepareResources();
	// create mesh
	boxMesh = new BoxMesh();
	circleMesh = new CircleMesh();


	// create shaders
	ShaderCompiler sc;
	sc.SetIsDebug(true);
	sc.SetEntryPoint("VSMain");
	sc.SetTarget("vs_5_0");
	sc.SetPathToShader(L"../../Shaders/MyVeryFirstShader.hlsl");

	vs = sc.CreateShader<SimpleVertexShader>();

	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");

	ps = sc.CreateShader<PixelShader>();

	sc.SetEntryPoint("PSPlainColor");
	psPlain = sc.CreateShader<PixelShader>();

	// Setup camera
	Camera* camera = new Camera();
	CurrentCamera = camera;

	camera->UpdateProjectionMatrixPerspective(45.0f, Display->GetAspectRatio(), 0.01f, 1000.0f);

	camera->Transform.Position = camera->Transform.Rotation.GetForwardVector() * -5.0f;


	Actor* box = new Actor();
	MeshRenderer* mr = box->AddActorComponent<MeshRenderer>();
	mr->SetMesh(boxMesh);
	mr->SetVertexShader(vs);
	mr->SetPixelShader(ps);
	box->Transform.Rotation.SetEulerAngles(0.0f, 0.0f, 30.0f);

	Actor* box2 = new Actor();
	mr = box2->AddActorComponent<MeshRenderer>();
	mr->SetMesh(boxMesh);
	mr->SetVertexShader(vs);
	mr->SetPixelShader(ps);
	RotateAroundActorComponent* raac = box2->AddActorComponent<RotateAroundActorComponent>();
	raac->ActorToRotateAround = box;
	raac->OrbitRadius = 3.0f;
	raac->Scale = Vector3::One * 0.5f;

	Actor* box3 = new Actor();
	mr = box3->AddActorComponent<MeshRenderer>();
	mr->SetMesh(boxMesh);
	mr->SetVertexShader(vs);
	mr->SetPixelShader(ps);
	raac = box3->AddActorComponent<RotateAroundActorComponent>();
	raac->ActorToRotateAround = box2;
	raac->Scale = Vector3::One * 0.5f;

	CameraController* camController = new CameraController();
	camController->SetCameraToControl(camera);



}