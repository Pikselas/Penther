#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include<string>

class Shader
{
 protected:
	 Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
 public:
	 virtual void Bind(ID3D11Device& Device, ID3D11DeviceContext& Context) const = 0;
};