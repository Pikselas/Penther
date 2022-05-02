#pragma once
#include"Shader.h"

class PixelShader : public Shader
{
 public:
	 PixelShader(const std::wstring& cso_file)
	 {
		 D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
	 }
	 void Bind(ID3D11Device& Device, ID3D11DeviceContext& Context) const override
	 {
		 Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		 Device.CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &shader);
		 Context.PSSetShader(shader.Get(), nullptr, 0u);
	 }
};