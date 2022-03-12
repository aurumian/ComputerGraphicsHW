#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "ActorComponent.h"

#include <DirectXMath.h>

class MeshRenderer : public ActorComponent
{
public:
	void SetMesh(Mesh* InMesh) { Mesh = InMesh; }

	void SetVertexShader(VertexShader* InVertexShader) { VertexShader = InVertexShader; }

	void SetPixelShader(PixelShader* InPixelShader) { PixelShader = InPixelShader; }

	void Render();

	void SetColor(XMFLOAT4 InColor);

private:
	Mesh* Mesh = nullptr;

	VertexShader* VertexShader = nullptr;

	PixelShader* PixelShader = nullptr;

	// todo: move to a material
	XMFLOAT4 Color;
};

