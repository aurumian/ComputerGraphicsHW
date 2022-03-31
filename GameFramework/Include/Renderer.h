#pragma once

#include "GameComponent.h"

class Renderer : public GameComponent
{
public:

	virtual void Render() = 0;

	void SetVertexShader(class VertexShader* InVertexShader) { mVertexShader = InVertexShader; }

	void SetPixelShader(class PixelShader* InPixelShader) { mPixelShader = InPixelShader; }

	void SetColor(Color InColor) { Color = InColor; }

protected:

	// todo: move to a material
	Color Color;

	class VertexShader* mVertexShader = nullptr;

	class PixelShader* mPixelShader = nullptr;
};