#pragma once

#include "GameComponent.h"

class Renderer : public GameComponent
{
public:

	virtual void Render() = 0;

	void SetVertexShader(class VertexShader* InVertexShader) { mVertexShader = InVertexShader; }

	void SetPixelShader(class PixelShader* InPixelShader) { mPixelShader = InPixelShader; }

	void SetColor(Color InColor) { mColor = InColor; }

	bool bCastShadow = true;

protected:

	// todo: move to a material
	Color mColor = Color(1.0f, 1.0f, 1.0, 1.0f);

	class VertexShader* mVertexShader = nullptr;

	class PixelShader* mPixelShader = nullptr;
};