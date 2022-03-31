#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"

#include <DirectXMath.h>

class MeshRenderer : public Renderer
{
public:
	void SetMesh(Mesh* InMesh) { Mesh = InMesh; }


	virtual void Render() override;

private:
	Mesh* Mesh = nullptr;


};

