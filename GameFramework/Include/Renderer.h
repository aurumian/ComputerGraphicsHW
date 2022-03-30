#pragma once

#include "GameComponent.h"

class Renderer : public GameComponent
{
public:

	virtual void Render() = 0;

	void SetVertexShader(class VertexShader* InVertexShader) { mVertexShader = InVertexShader; }

	void SetPixelShader(class PixelShader* InPixelShader) { mPixelShader = InPixelShader; }

protected:

	class VertexShader* mVertexShader = nullptr;

	class PixelShader* mPixelShader = nullptr;
};