#include "MeshRenderer.h"

#include "Game.h"
#include "Shader.h"
#include "MeshProxy.h"

#include "Mesh.h"

#include <d3d11.h>

void MeshRenderer::SetMeshProxy(MeshProxy* InMeshProxy)
{
	mMeshProxy = InMeshProxy;
}

void MeshRenderer::SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV)
{
	mAlbedoSRV = InAlbedoSRV;
}

void MeshRenderer::Render(const RenderingSystemContext& RSContext)
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
	mVertexShader->UseShader(static_cast<ShaderFlag>(RSContext.ShaderFlags));
	// todo: render the scene with override material instead of using a bool
	if (game->bIsRenderingShadowMap)
	{
		context->PSSetShader(nullptr, nullptr, 0);
	}
	else
	{
		mPixelShader->UseShader(static_cast<ShaderFlag>(RSContext.ShaderFlags));
	}

	// Update constant buffer with world matrix
	CBPerObject cbData;
	cbData.ObjectToWorld = GetWorldTransform().GetTransformMatrixTransposed();
	cbData.Color = mColor;
	cbData.NormalObjectToWorld = GetWorldTransform().GetNormalMatrixTransposed();
	cbData.Mat = Mat;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	
	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	context->IASetIndexBuffer(mMeshProxy->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, mMeshProxy->GetVertexBuffer().GetAddressOf(), mMeshProxy->GetStrides(), mMeshProxy->GetOffsets());

	context->PSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());
	context->VSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());

	// Textures
	if (mAlbedoSRV != nullptr)
	context->PSSetShaderResources(0, 1, mAlbedoSRV.GetAddressOf());
	context->PSSetSamplers(0, 1, defaultSamplerState.GetAddressOf());

	if (mNormalSRV != nullptr)
	context->PSSetShaderResources(2, 1, mNormalSRV.GetAddressOf());

	if (mSpecularSRV != nullptr)
	context->PSSetShaderResources(3, 1, mSpecularSRV.GetAddressOf());


	if (!game->bIsRenderingShadowMap)
	{
		context->PSSetShaderResources(1, 1, game->GetShadowMapSRV().GetAddressOf());
		context->PSSetSamplers(1, 1, game->GetShadowmapSamplerState().GetAddressOf());
	}

	context->DrawIndexed(mMeshProxy->GetNumIndices(), 0, 0);
}
