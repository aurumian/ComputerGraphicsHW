#pragma once

#include <wrl/client.h>

#include <d3d.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

#include "Camera.h"

using namespace Microsoft::WRL;

#pragma pack(push, 4)
struct CBPerObject
{
	DirectX::XMFLOAT4X4 ObjectToWorld;
	DirectX::XMFLOAT4 Color;
};

struct CBPerDraw
{
	Matrix WorldToClip;
};
#pragma pack(pop)

//template<class T> 
//T* CreateGameInstance()
//{
//	T* game = new T();
//	T::Instance = game;
//	return game;
//}

class Game
{

public:
	friend int main();
	friend class Actor;

	void Initialize();

	void CreateBackBuffer();

	void Run();

	void Exit();

	LRESULT HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	virtual void PrepareResources();

	ComPtr<ID3D11Device> GetD3DDevice();

	ComPtr<ID3D11DeviceContext> GetD3DDeviceContext();

	ComPtr<ID3D11Buffer> GetPerObjectConstantBuffer();

	void UpdateInternal(float DeltaTime);
	virtual void Update(float DeltaTime);
	

	void Render();

	virtual void DestroyResources();

	class InputDevice* GetInputDevice();
	class DisplayWin32* GetDisplay();

	class Collider* GetOverlapping(const class Collider* Col);
	
	template<class T>
	T* CreateGameComponent(class GameComponent* Parent = nullptr)
	{
		T* comp = new T();

		GameComponents.push_back(comp);

		comp->SetParent(Parent);

		if constexpr (std::is_base_of<class Renderer, T>::value)
		{
			Renderers.push_back(comp);
		}
		else if constexpr (std::is_base_of<class Collider, T>::value)
		{
			Colliders.push_back(comp);
		}

		return comp;
	}

	/*
	
	void EndFrame();
	
	
	
	
	void PrepareFrame();
	
	void RestoreTargets();
	
	
	*/


	static Game* GetInstance();

	float GetTotalElapsedTime();

	virtual ~Game();

	const Camera& GetCurrentCamera();

	ComPtr<ID3D11SamplerState> GetDefaultSamplerState() { return DefaultSamplerState; }

	void DestroyComponent(GameComponent* GC);

protected:

	Game();

	std::vector<class Renderer*> Renderers;
	std::vector<class Collider*> Colliders;

	std::vector<class GameComponent*> GameComponents;

	Camera* CurrentCamera = nullptr;

	class DisplayWin32* Display = nullptr;

	ComPtr<ID3D11Texture2D> pDepthStencil = nullptr;

	ComPtr<ID3D11DepthStencilState> pDSState = nullptr;
	ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;

	ComPtr<ID3D11SamplerState> DefaultSamplerState = nullptr;

private:

	
	class InputDevice* Input = nullptr;

	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;

	ComPtr<ID3D11Texture2D> BackBuf;

	ComPtr<ID3D11RenderTargetView> RenderTargetView;

	ComPtr<ID3D11Buffer> PerDrawCB;
	ComPtr<ID3D11Buffer> PerObjectCB;

	bool ExitRequested = false;

	

	static Game* Instance;


	float StartTime = 0.0f;

	Camera DefaultCamera = Camera();


};

