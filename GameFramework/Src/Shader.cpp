#include "Shader.h"

#include "Game.h"

ComPtr<ID3DBlob>& Shader::GetByteCodeRef()
{
    return ByteCode;
}

// todo: update to return bool or hresult and handle errors
void VertexShader::Initialize()
{
	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreateVertexShader(
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		nullptr, 
		D3DShaderPointer.GetAddressOf());
}

void VertexShader::UseShader()
{
	ComPtr<ID3D11DeviceContext> context = Game::GetInstance()->GetD3DDeviceContext();

	context->VSSetShader(D3DShaderPointer.Get(), nullptr, 0);
	context->IASetInputLayout(InputLayout.Get());
}

void PixelShader::Initialize()
{
	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreatePixelShader(
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		nullptr,
		D3DShaderPointer.GetAddressOf());
}

void PixelShader::UseShader()
{
	ComPtr<ID3D11DeviceContext> context = Game::GetInstance()->GetD3DDeviceContext();

	context->PSSetShader(D3DShaderPointer.Get(), nullptr, 0);
}

void BasicVertexShader::Initialize()
{
	VertexShader::Initialize();

	D3D11_INPUT_ELEMENT_DESC inputElements[] =
	{
		D3D11_INPUT_ELEMENT_DESC
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreateInputLayout(
		inputElements,
		1,
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		InputLayout.GetAddressOf()
	);
}

void SimpleVertexShader::Initialize()
{
	VertexShader::Initialize();

	D3D11_INPUT_ELEMENT_DESC inputElements[] =
	{
		D3D11_INPUT_ELEMENT_DESC
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreateInputLayout(
		inputElements,
		2,
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		InputLayout.GetAddressOf()
	);
}


