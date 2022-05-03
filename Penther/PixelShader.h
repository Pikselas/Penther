#pragma once
#include"Canvas3D.h"

class PixelShader : public Shader<Canvas3D>
{
 private:
	 Microsoft::WRL::ComPtr<ID3D11PixelShader> SHADER;
 public:
	 PixelShader(const Canvas3D& c3d, const std::wstring& cso_file , std::optional<ConstantBuffer> c_buffer = {}) : Shader(c_buffer)
	 {
		 Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
		 D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
		 GetDevice(c3d).CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
	 }
	 void Bind(const Canvas3D& c3d) const override
	 {
		 auto& Context = GetContext(c3d);
		 Context.PSSetShader(SHADER.Get(), nullptr, 0u);
		 if (C_BUFFER)
		 {
			 Context.PSSetConstantBuffers(0u, 1u, C_BUFFER->GetAddressOf());
		 }
	 }
};