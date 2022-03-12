#pragma once

#include <d3dcommon.h>
#include <d3d11.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace DirectX
{
	struct XMFLOAT4;
}

class Mesh
{
	friend class MeshRenderer;

public:

	void Initialize(struct DirectX::XMFLOAT4* Points, size_t NumPoints, int* Indices, size_t NumIndices);

	// todo: add check for initialization
	// mb: i should move buffers to MeshRenderer

	int indexCount = 0;

protected:

	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;
	

};

// todo: move to PongMeshes.h and PongMeshes.cpp files
#include <DirectXMath.h>
#include <xutility>
using namespace DirectX;
class SquareMesh : public Mesh
{
public:
	SquareMesh()
	{
		XMFLOAT4 points[8] =
		{
			XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(-1.0f, -1.0f, 0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, -1.0f, 0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		int indices[] = { 0, 1, 2, 1, 0, 3 };

		Initialize(points, std::size(points), indices, std::size(indices));
	}
};

#include <vector>
class CircleMesh : public Mesh
{
public:
	CircleMesh()
	{

		std::vector<XMFLOAT4> points;
		std::vector<int> indices;
		points.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });
		points.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

		int numPoints = 20;
		int i = 1;
		for (float alpha = 0.0f; alpha < 6.28f; alpha += 6.28f / numPoints, ++i)
		{
			XMFLOAT4 point = XMFLOAT4(cosf(alpha), sinf(alpha), 0.0f, 1.0f);
			points.push_back(point);
			points.push_back(XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f));
			indices.push_back(0);
			indices.push_back(i);
			int ind2 = (i + 1) % (numPoints + 1);
			if (ind2 == 0)
			{
				ind2 = 1;
			}
			indices.push_back(ind2);
		}

		Initialize(&points[0], points.size(), &indices[0], indices.size());
	}
};

class BoxMesh : public Mesh
{
public:
	BoxMesh()
	{
		XMFLOAT4 points[16] =
		{
			XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),		// 0
			XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	// 1
			XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	// 2
			XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 3
			XMFLOAT4(0.5f, 0.5f, -0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	// 4
			XMFLOAT4(-0.5f, -0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	// 5
			XMFLOAT4(0.5f, -0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	// 6
			XMFLOAT4(-0.5f, 0.5f, -0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 7
		};

		int indices[] = 
		{ 
			0, 1, 2, 
			1, 0, 3, 
			4, 5, 6,
			5, 4, 7,
			4, 0, 2,
			4, 2, 6,
			7, 5, 1,
			7, 1, 3,
			4, 0, 3,
			4, 3, 7,
			6, 1, 2,
			6, 5, 1
		};

		Initialize(points, std::size(points), indices, std::size(indices));
	}
};