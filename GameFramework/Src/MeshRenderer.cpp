#include "MeshRenderer.h"

#include "Game.h"

#include <d3d11.h>

void MeshRenderer::Render()
{
	if (VertexShader == nullptr || PixelShader == nullptr || Mesh == nullptr)
	{
		return;
	}

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// todo: optimize rendering by 
	// checking which shader is set now
	// and/or sorting meshes by used shaders
	VertexShader->UseShader();
	PixelShader->UseShader();

	// Update constant buffer with world matrix
	CBPerObject cbData;
	cbData.ObjectToWorld = GetWorldTransform().GetTransformMatrixTransposed();
	cbData.Color = Color;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	
	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	// todo: move this to mesh?
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	context->IASetIndexBuffer(Mesh->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, Mesh->VertexBuffer.GetAddressOf(), strides, offsets);

	context->DrawIndexed(Mesh->indexCount, 0, 0);
}

void MeshRenderer::SetColor(XMFLOAT4 InColor)
{
	Color = InColor;
}
