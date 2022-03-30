#include "PlaneComponent.h"

#include "Shader.h"

#include <vector>

using namespace std;

PlaneComponent::PlaneComponent()
{	
	const int gridSize = 20;
	const float distBetweenLines = 4.0f;
	const float farthestCoord = gridSize / 2.0f * distBetweenLines;
	std::vector<Vector4> points;
	for (int i = 0; i <= gridSize; ++i)
	{
		// Add vertical line
		points.push_back(Vector4(i * distBetweenLines - farthestCoord, 0.0f, -farthestCoord, 1.0f));
		points.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		points.push_back(Vector4(i * distBetweenLines - farthestCoord, 0.0f, farthestCoord, 1.0f));
		points.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// Add horizontal lines
	for (int i = 0; i <= gridSize; ++i)
	{
		// add vertical line
		points.push_back(Vector4(-farthestCoord, 0.0f, i * distBetweenLines - farthestCoord, 1.0f));
		points.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		points.push_back(Vector4(farthestCoord, 0.0f, i * distBetweenLines - farthestCoord, 1.0f));
		points.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	numVerts = points.size() / 2;

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(points[0]) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = &points[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
	device->CreateBuffer(&vertexBufDesc, &vertexData, VertexBuffer.GetAddressOf());
}

void PlaneComponent::Render()
{

	if (mVertexShader == nullptr || mPixelShader == nullptr)
	{
		return;
	}

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// todo: optimize rendering by 
	// checking which shader is set now
	// and/or sorting meshes by used shaders
	mVertexShader->UseShader();
	mPixelShader->UseShader();

	// Update constant buffer with world matrix
	CBPerObject cbData;
	cbData.ObjectToWorld = GetWorldTransform().GetTransformMatrixTransposed();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	// todo: move this to mesh?
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };


	context->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), strides, offsets);

	context->Draw(numVerts, 0);

}
