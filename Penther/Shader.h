#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include<optional>
#include"ConstantBuffer.h"

class Shader
{
 protected:
	 std::optional<Microsoft::WRL::ComPtr<ID3D11Buffer>> C_BUFFER;
protected:
	Shader(std::optional<ConstantBuffer> c_buffer = {})
	{
		if (c_buffer)
		{
			C_BUFFER = c_buffer->BUFFER;
		}
	}
 public:
	 virtual void Bind(ID3D11DeviceContext&) const = 0;
};

