#pragma once

#include <string>
#include <d3dcommon.h>

#include "Shader.h"

class ShaderCompiler
{

public:
	ShaderCompiler() = default;

	bool Compile(ID3DBlob** OutResult);

	template<class T>
	T* CreateShader(bool Initialize = true)
	{
		T* shader = new T();

		if (!Compile(shader->GetByteCodeRef().GetAddressOf()))
		{
			delete shader;
			return nullptr;
		}

		shader->Initialize();
		return shader;
	}

	void SetPathToShader(const std::wstring& Path);

	void SetIsDebug(const bool InIsDebug);

	void SetEntryPoint(const std::string& InEntryPoint);

	void SetTarget(const std::string& InTarget);


private:

	std::wstring PathToShader;

	bool IsDebug = false;

	std::string EntryPoint;

	std::string Target;


	
};

