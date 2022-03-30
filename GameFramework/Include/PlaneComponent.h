#pragma once

#include "Renderer.h"
#include <vector>

class PlaneComponent : public Renderer
{
public:
	PlaneComponent();

	virtual void Render() override;

protected:
	ComPtr<ID3D11Buffer> VertexBuffer;
	//ID3D11Buffer* VertexBuffer;

	int numVerts;
};