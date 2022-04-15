#include "Mesh.h"

#include "Game.h"
#include "MeshProxy.h"

MeshProxy* ColoredMesh::CreateMeshProxy()
{
	MeshProxy* meshProxy = new MeshProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(),  sizeof(Vertex),  &Indices[0], Indices.size());

	return meshProxy;
}

UINT ColoredMesh::AddVertex(const Vertex& InVertex)
{
	Vertices.push_back(InVertex);
	return static_cast<UINT>(Vertices.size() - 1);
}


void ColoredMesh::AddIndex(UINT InIndex)
{
	Indices.push_back(InIndex);
}

void ColoredMesh::AddFace(UINT I1, UINT I2, UINT I3)
{
	Indices.push_back(I1);
	Indices.push_back(I2);
	Indices.push_back(I3);
}

MeshProxy* TexturedMesh::CreateMeshProxy()
{
	MeshProxy* meshProxy = new MeshProxy();

	meshProxy->Initialize(&Vertices[0], Vertices.size(), sizeof(Vertex), &Indices[0], Indices.size());

	return meshProxy;
}

UINT TexturedMesh::AddVertex(const Vertex& InVertex)
{
	Vertices.push_back(InVertex);
	return static_cast<UINT>(Vertices.size() - 1);
}

void TexturedMesh::AddIndex(UINT InIndex)
{
	Indices.push_back(InIndex);
}

TexturedMesh::Vertex& TexturedMesh::GetVertex(size_t index)
{
	return Vertices[index];
}
