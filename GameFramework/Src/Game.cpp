#include "Game.h"

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "MeshRenderer.h"
#include "ShaderCompiler.h"
#include "AABB2DCollider.h"

#include <chrono>

Game* Game::Instance = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::GetInstance()->HandleMessage(hwnd, umessage, wparam, lparam);
}

void Game::Initialize()
{
	StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0f;

	Display = new DisplayWin32(800, 1200, &WndProc, L"PONG");
	Input = new InputDevice();

	CreateBackBuffer();

	PrepareResources();
	

}

void Game::CreateBackBuffer()
{
	D3D_FEATURE_LEVEL featureLevel[]{ D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->GetClientWidth();
	swapDesc.BufferDesc.Height = Display->GetClientHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->GetWindowHandle();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf());

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuf.GetAddressOf()));
	res = Device->CreateRenderTargetView(BackBuf.Get(), nullptr, RenderTargetView.GetAddressOf());

	// Step 11. Set back buffer for output
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->GetClientWidth());
	viewport.Height = static_cast<float>(Display->GetClientHeight());
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);

	// todo: move to mesh material
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rastState;
	res = Device->CreateRasterizerState(&rastDesc, &rastState);

	Context->RSSetState(rastState);

	//Create per draw constant buffer.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	res = Device->CreateBuffer(&cbDesc, nullptr,
		PerDrawCB.GetAddressOf());

	Context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());
	Context->PSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());


	//Create per object constant buffer.
	res = Device->CreateBuffer(&cbDesc, nullptr,
		PerObjectCB.GetAddressOf());

	Context->VSSetConstantBuffers(1, 1, PerObjectCB.GetAddressOf());
	Context->PSSetConstantBuffers(1, 1, PerObjectCB.GetAddressOf());

	
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = Display->GetClientWidth();
	descDepth.Height = Display->GetClientHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	res = Device->CreateTexture2D(&descDepth, NULL, pDepthStencil.GetAddressOf());

	//D3D11_DEPTH_STENCIL_DESC dsDesc;

	//// Depth test parameters
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//// Stencil test parameters
	//dsDesc.StencilEnable = true;
	//dsDesc.StencilReadMask = 0xFF;
	//dsDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Create depth stencil state
	//Device->CreateDepthStencilState(&dsDesc, pDSState.GetAddressOf());
	
	//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	//descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view

	res = Device->CreateDepthStencilView(pDepthStencil.Get(), // Depth stencil texture
		nullptr, // Depth stencil desc
		DepthStencilView.GetAddressOf());  // [out] Depth stencil view


	// Create default sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	GetD3DDevice()->CreateSamplerState(&sampDesc, DefaultSamplerState.GetAddressOf());
}

void Game::Run()
{
	Initialize();

	auto PrevTime = std::chrono::steady_clock::now();


	MSG msg = {};
	while (!ExitRequested)
	{
		auto now = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - PrevTime).count() / 1000000.0f;
		PrevTime = now;

		// Process input
		Input->PreInputProcess();
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			Exit();
		}

		// Update
		UpdateInternal(deltaTime);

		// Render
		Render();

		
	}
}

void Game::Exit()
{
	ExitRequested = true;
}

LRESULT Game::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Input->HandleMessage(hwnd, umessage, wparam, lparam);
}

void Game::PrepareResources()
{

}

ComPtr<ID3D11Device> Game::GetD3DDevice()
{
	return Device;
}

ComPtr<ID3D11DeviceContext> Game::GetD3DDeviceContext()
{
	return Context;
}

ComPtr<ID3D11Buffer> Game::GetPerObjectConstantBuffer()
{
	return PerObjectCB;
}

void Game::UpdateInternal(float DeltaTime)
{
	for (GameComponent* gc : GameComponents)
	{
		if (gc != nullptr && gc->bShouldUpdate)
		{
			gc->Update(DeltaTime);
		}
	}
	
	Update(DeltaTime);
}

void Game::Update(float DeltaTime)
{
	
}

void Game::Render()
{
	// Bind the depth stencil view
	Context->OMSetRenderTargets(1,          // One rendertarget view
		RenderTargetView.GetAddressOf(),      // Render target view, created earlier
		DepthStencilView.Get());     // Depth stencil view for the render target
	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	//float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Bind depth stencil state
	//
	//Context->ClearDepthStencilView();
	Context->ClearRenderTargetView(RenderTargetView.Get(), clearColor);
	Context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// Update world to clip matrix
	CBPerDraw cbData;
	const Camera& cam = GetCurrentCamera();
	cbData.WorldToClip = cam.GetWorldToClipMatrix();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = Context->Map(PerDrawCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	Context->Unmap(PerDrawCB.Get(), 0);

	for (Renderer* renderer : Renderers)
	{
		if (renderer != nullptr)
		{
			renderer->Render();
		}
	}

	SwapChain->Present(1, 0);
}

void Game::DestroyResources()
{
	delete Display; 
	delete Input;
}

InputDevice* Game::GetInputDevice()
{
	return Input;
}

DisplayWin32* Game::GetDisplay()
{
	return Display;
}

Collider* Game::GetOverlapping(const Collider* Col)
{
	if (Col == nullptr)
	{
		return nullptr;
	}
	for (Collider* c : Colliders)
	{
		if (c == Col)
		{
			continue;
		}
		if (Col->DoesOverlap(*c))
		{
			return c;
		}
	}
	return nullptr;
}

Game* Game::GetInstance()
{
	return Instance;
}

float Game::GetTotalElapsedTime()
{
	float now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000.0f;

	return now - StartTime;
}

Game::~Game()
{
	for (GameComponent* gc : GameComponents)
	{
		delete gc;
	}
	Context->Flush();
}

const Camera& Game::GetCurrentCamera()
{
	if (CurrentCamera == nullptr)
	{
		return DefaultCamera;
	}

	return *CurrentCamera;
}

void Game::DestroyComponent(GameComponent* GC)
{
	if (GC == nullptr)
	{
		return;
	}

	GC->SetParent(nullptr);
	for (GameComponent* gc : GC->Children)
	{
		if (gc != nullptr)
		{
			gc->SetParent(nullptr);
		}
	}

	GameComponents.erase(remove(GameComponents.begin(), GameComponents.end(), GC), GameComponents.end());
	
	if (Collider* col = dynamic_cast<Collider*>(GC))
	{
		Colliders.erase(remove(Colliders.begin(), Colliders.end(), col), Colliders.end());
	}
	else if (Renderer* rend = dynamic_cast<Renderer*>(GC))
	{
		Renderers.erase(remove(Renderers.begin(), Renderers.end(), rend), Renderers.end());
	}

	delete GC;
}

Game::Game()
{
	Instance = this;
}
