#pragma once

#include "Renderer.h"

#include <DirectXMath.h>

class MeshRenderer : public Renderer
{
public:
	void SetMeshProxy(class MeshProxy* InMeshProxy);

	virtual void Render() override;

protected:
	class MeshProxy* mMeshProxy = nullptr;


};

