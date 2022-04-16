#include "MeshRenderer.h"

#include "Game.h"
#include "Shader.h"
#include "MeshProxy.h"

#include <d3d11.h>

void MeshRenderer::SetMeshProxy(MeshProxy* InMeshProxy)
{
	mMeshProxy = InMeshProxy;
}

void MeshRenderer::SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV)
{
	mAlbedoSRV = InAlbedoSRV;
}

void MeshRenderer::Render()
{
	if (mVertexShader == nullptr || mPixelShader == nullptr || mMeshProxy == nullptr)
	{
		return;
	}

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();
	ComPtr<ID3D11SamplerState> defaultSamplerState = game->GetDefaultSamplerState();

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// todo: optimize rendering by 
	// checking which shader is set now
	// and/or sorting meshes by used shaders
	mVertexShader->UseShader();
	mPixelShader->UseShader();

	// Update constant buffer with world matrix
	CBPerObject cbData;
	cbData.ObjectToWorld = GetWorldTransform().GetTransformMatrixTransposed();
	cbData.Color = Color;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	
	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	context->IASetIndexBuffer(mMeshProxy->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, mMeshProxy->GetVertexBuffer().GetAddressOf(), mMeshProxy->GetStrides(), mMeshProxy->GetOffsets());

	// Textures
	if (mAlbedoSRV.Get() != nullptr)
	{
		context->PSSetShaderResources(0, 1, mAlbedoSRV.GetAddressOf());
		context->PSSetSamplers(0, 1, defaultSamplerState.GetAddressOf());
	}

	context->DrawIndexed(mMeshProxy->GetNumIndices(), 0, 0);
}
