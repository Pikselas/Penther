#pragma once
#include"Canvas3D.h"
#include"CanvasAccesser.h"
#include"Texture.h"

class PixelShader : public Shader , CanvasAccesser
{
 private:
	 Microsoft::WRL::ComPtr<ID3D11PixelShader> SHADER;
	 std::optional<Texture> TEXTURE;
 public:
	 PixelShader(const Canvas3D& c3d, const std::wstring& cso_file , std::optional<ConstantBuffer> c_buffer = {}) : Shader(c_buffer)
	 {
		 Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
		 D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
		 GetDevice(c3d).CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
	 }
	 void SetTexture(const Texture& tex)
	 {
		 TEXTURE = tex;
	 }
	 void Bind(ID3D11DeviceContext& Context) const override
	 {
		 Context.PSSetShader(SHADER.Get(), nullptr, 0u);
		 if (C_BUFFER)
		 {
			 Context.PSSetConstantBuffers(0u, 1u, C_BUFFER->GetAddressOf());
		 }
		 if (TEXTURE)
		 {
			 Context.PSSetShaderResources(0u, 1u, TEXTURE->TEXTURE_VIEW.GetAddressOf());
			 Context.PSSetSamplers(0u, 1u, TEXTURE->SAMPLER.GetAddressOf());
		 }
	 }
};