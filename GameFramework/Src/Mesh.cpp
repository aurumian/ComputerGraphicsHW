#include "Mesh.h"

#include "Game.h"

#include <DirectXMath.h>

using namespace DirectX;

void Mesh::Initialize(XMFLOAT4* Points, size_t NumPoints, int* Indices, size_t NumIndices)
{
	indexCount = static_cast<int>(NumIndices);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Points[0]) * static_cast<UINT>(NumPoints);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = Points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
	device->CreateBuffer(&vertexBufDesc, &vertexData, VertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(Indices[0]) * static_cast<UINT>(NumIndices);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = Indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, IndexBuffer.GetAddressOf());
}
