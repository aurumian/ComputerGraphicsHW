#pragma once

#include "Renderer.h"

#include <DirectXMath.h>

class MeshRenderer : public Renderer
{
public:
	void SetMeshProxy(class MeshProxy* InMeshProxy);

	void SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV);

	virtual void Render() override;

protected:
	class MeshProxy* mMeshProxy = nullptr;

	ComPtr<ID3D11ShaderResourceView> mAlbedoSRV = nullptr;
};

