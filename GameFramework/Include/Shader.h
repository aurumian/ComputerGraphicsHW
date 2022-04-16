#pragma once

#include <d3dcommon.h>
#include <d3d11.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

class Shader
{
public:

	ComPtr<ID3DBlob>& GetByteCodeRef();

	virtual void Initialize() = 0;

	virtual void UseShader() = 0;

protected:
	ComPtr<ID3DBlob> ByteCode;

	

	friend class ShaderCompiler;
};

class VertexShader : public Shader
{
public:
	//ComPtr<ID3D11VertexShader> GetD3DShaderPointer();

	virtual void Initialize() override;

	virtual void UseShader() override;

protected: 
	ComPtr<ID3D11VertexShader> D3DShaderPointer;

	ComPtr<ID3D11InputLayout> InputLayout;
};

class PixelShader : public Shader
{
public:
	//ComPtr<ID3D11PixelShader> GetD3DShaderPointer();

	virtual void Initialize() override;

	virtual void UseShader() override;

protected:
	ComPtr<ID3D11PixelShader> D3DShaderPointer;
};

class BasicVertexShader : public VertexShader
{
public:
	virtual void Initialize();
};

// todo: move to a separate file
class SimpleVertexShader : public VertexShader
{
public:
	virtual void Initialize();
};

class TexturedVertexShader : public VertexShader
{
public:
	virtual void Initialize();
};