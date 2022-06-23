#include "Shader.h"

#include "Game.h"

LPCSTR MacroNames[static_cast<int>(ShaderFlag::MAX) - 1] =
{
	"FORWARD_RENDERING",
	"DEFERRED_OPAQUE",
	"DIRECTIONAL_LIGHT"
};

LPCSTR GetFlagString(ShaderFlag Flags)
{
	const int flagsAsInt = static_cast<int>(Flags);
	if (flagsAsInt % 2 && flagsAsInt != 1)
	{
		return nullptr;
	}

	return MacroNames[flagsAsInt - 1];
}


// todo: update to return bool or hresult and handle errors
void VertexShader::Initialize(ID3DBlob* ByteCode, ShaderFlag Flags)
{
	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreateVertexShader(
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		nullptr, 
		ShaderVariations.emplace(Flags, nullptr).first->second.GetAddressOf());
}

void VertexShader::UseShader(ShaderFlag Flags)
{
	ComPtr<ID3D11DeviceContext> context = Game::GetInstance()->GetD3DDeviceContext();

	// @TODO: Check if variation actually exists?
	context->VSSetShader(ShaderVariations[Flags].Get(), nullptr, 0);
	context->IASetInputLayout(InputLayout.Get());
}

void PixelShader::Initialize(ID3DBlob* ByteCode, ShaderFlag Flags)
{
	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreatePixelShader(
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		nullptr,
		ShaderVariations.emplace(Flags, nullptr).first->second.GetAddressOf());
}

void PixelShader::UseShader(ShaderFlag Flags)
{
	ComPtr<ID3D11DeviceContext> context = Game::GetInstance()->GetD3DDeviceContext();

	context->PSSetShader(ShaderVariations[Flags].Get(), nullptr, 0);
}

void BasicVertexShader::Initialize(ID3DBlob* ByteCode, ShaderFlag Flags)
{
	VertexShader::Initialize(ByteCode, Flags);

	// @TODO: Create input layout only once on parent class level?
	if (Flags != ShaderFlag::None)
	{
		return;
	}

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

void SimpleVertexShader::Initialize(ID3DBlob* ByteCode, ShaderFlag Flags)
{
	VertexShader::Initialize(ByteCode, Flags);

	// @TODO: Create input layout only once on parent class level?
	if (Flags != ShaderFlag::None)
	{
		return;
	}

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

void TexturedVertexShader::Initialize(ID3DBlob* ByteCode, ShaderFlag Flags)
{
	VertexShader::Initialize(ByteCode, Flags);

	// @TODO: Create input layout only once on parent class level?
	if (Flags != ShaderFlag::None)
	{
		return;
	}

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
		},
		D3D11_INPUT_ELEMENT_DESC
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC
		{
			"BINORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC
		{
			"TANGENT",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();

	device->CreateInputLayout(
		inputElements,
		5,
		ByteCode->GetBufferPointer(),
		ByteCode->GetBufferSize(),
		InputLayout.GetAddressOf()
	);
}
