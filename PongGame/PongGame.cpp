#include "PongGame.h"

void BallActor::Update(float DeltaTime)
{
	{
		DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&Direction);
		vec = DirectX::XMVector3Normalize(vec);
		DirectX::XMStoreFloat3(&Direction, vec);

		Speed += Acceleration * DeltaTime;

		const float speedPerFrame = Speed * DeltaTime;

		Transform.Position.x += speedPerFrame * Direction.x;
		Transform.Position.y += speedPerFrame * Direction.y;
		//Transform.Position.z += Speed * Direction.z;

		// check overlap
		// if overlapping border, then inverse y direction
		// if overlapping player inverse x direction
		// if overlapping player back move the ball to center and update the Score
		Collider* col = Game::GetInstance()->GetOverlapping(GetComponent<AABB2DCollider>());

		if (col != nullptr && !WaitForEndOverlap)
		{

			PongActor* pa = dynamic_cast<PongActor*>(col->GetOwner());
			if (pa != nullptr)
			{
				switch (pa->PongType)
				{
				case PongType::Border:
					Direction.y = -Direction.y;
					break;
				case PongType::Player:
					Direction.x = -Direction.x;
					break;
				case PongType::Player1Behind:
				case PongType::Player2Behind:
					class PongGame* game = dynamic_cast<PongGame*>(Game::GetInstance());
					if (game != nullptr)
					{
						game->PlayerScored(pa->PongType);
					}
					break;
				}
				WaitForEndOverlap = true;
			}
		}
		if (col == nullptr && WaitForEndOverlap)
		{
			WaitForEndOverlap = false;
		}

		if (abs(Transform.Position.x) > 2.0f || abs(Transform.Position.y) > 2.0f)
		{
			Transform.Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			ResetSpeed();
		}

	}
}

void PongGame::PrepareResources()
{
	Game::PrepareResources();
	// create mesh
	squareMesh = new SquareMesh();
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

	// Create borders
	BorderLeft = new PongActor();
	BorderRight = new PongActor();
	BorderTop = new PongActor();
	BorderBottom = new PongActor();

	const float borderWidth = 0.03f;

	MeshRenderer* mr = BorderLeft->AddActorComponent<MeshRenderer>();
	mr->SetMesh(squareMesh);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderLeft->Transform.Scale.x = borderWidth;
	BorderLeft->Transform.Position.x = -1 + BorderLeft->Transform.Scale.x / 2.0f;
	BorderLeft->AddActorComponent<AABB2DCollider>();
	BorderLeft->PongType = PongType::Player1Behind;

	mr = BorderRight->AddActorComponent<MeshRenderer>();
	mr->SetMesh(squareMesh);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderRight->Transform.Scale.x = borderWidth;
	BorderRight->Transform.Position.x = 1 - BorderLeft->Transform.Scale.x / 2.0f;
	BorderRight->AddActorComponent<AABB2DCollider>();
	BorderRight->PongType = PongType::Player2Behind;


	mr = BorderTop->AddActorComponent<MeshRenderer>();
	mr->SetMesh(squareMesh);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderTop->Transform.Scale.y = borderWidth * 12.0f / 8.0f;
	BorderTop->Transform.Position.y = 1 - BorderTop->Transform.Scale.y / 2.0f;
	BorderTop->AddActorComponent<AABB2DCollider>();
	BorderTop->PongType = PongType::Border;


	mr = BorderBottom->AddActorComponent<MeshRenderer>();
	mr->SetMesh(squareMesh);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	BorderBottom->Transform.Scale.y = borderWidth * 12.0f / 8.0f;
	BorderBottom->Transform.Position.y = -1 + BorderBottom->Transform.Scale.y / 2.0f;
	BorderBottom->AddActorComponent<AABB2DCollider>();
	BorderBottom->PongType = PongType::Border;


	// create players
	player1 = new PlayerActor();
	mr = player1->AddActorComponent<MeshRenderer>();
	mr->SetMesh(squareMesh);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.4f, 0.4f, 0.1f, 1.0f });
	player1->Transform.Scale.y = 0.3f;
	player1->Transform.Scale.x = borderWidth;
	player1->Transform.Position.x = -0.7f;
	player1->SetControls(87, 83);
	player1->AddActorComponent<AABB2DCollider>();

	player2 = new PlayerActor();
	mr = player2->AddActorComponent<MeshRenderer>();
	mr->SetMesh(squareMesh);
	mr->SetPixelShader(psPlain);
	mr->SetVertexShader(vs);
	mr->SetColor({ 0.1f, 0.4f, 0.4f, 1.0f });
	player2->Transform.Scale.y = 0.3f;
	player2->Transform.Scale.x = borderWidth;
	player2->Transform.Position.x = +0.7f;
	//player2->Transform.Rotation.SetEulerAngles({ 0.0f, 0.0f, 45.0f });
	player2->SetControls(38, 40);
	player2->AddActorComponent<AABB2DCollider>();

	ball = new BallActor();
	mr = ball->AddActorComponent<MeshRenderer>();

	mr->SetMesh(circleMesh);
	mr->SetPixelShader(ps);
	mr->SetVertexShader(vs);
	//mr->SetColor({ 0.1f, 0.4f, 0.1f, 1.0f });
	ball->Transform.Scale = Vector3(0.02f, 0.02f, 0.02f);
	ball->AddActorComponent<AABB2DCollider>();
}
