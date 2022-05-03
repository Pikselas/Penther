#pragma once
#include<vector>
#include<optional>

#include"Canvas3D.h"

struct InputElemDesc
{
public:
	enum class INPUT_FORMAT
	{
		FLOAT3 = DXGI_FORMAT_R32G32B32_FLOAT,
		FLOAT4 = DXGI_FORMAT_R32G32B32A32_FLOAT,
		UINT4 = DXGI_FORMAT_R8G8B8A8_UNORM
	};
public:
	INPUT_FORMAT FORMAT;
	std::string SEMANTIC_NAME;
	unsigned int OFFSET;
};

class VertexShader : public Shader<Canvas3D>
{
 private:
	 Microsoft::WRL::ComPtr<ID3D11VertexShader> SHADER;
	 Microsoft::WRL::ComPtr<ID3D11InputLayout> INPUT_LAYOUT;
  public:
	  VertexShader(const Canvas3D& c3d , const std::wstring& cso_file , const std::vector<InputElemDesc>& descs , std::optional<ConstantBuffer> c_buffer = {}) : Shader(c_buffer)
	  {
		  Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
		  D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
		  std::vector<D3D11_INPUT_ELEMENT_DESC> iedescs;
		  for (const auto& desc : descs)
		  {
			  iedescs.emplace_back(desc.SEMANTIC_NAME.c_str() , 0 , static_cast<DXGI_FORMAT>(desc.FORMAT) , 0 , desc.OFFSET , D3D11_INPUT_PER_VERTEX_DATA , 0);
		  }
		  GetDevice(c3d).CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
		  GetDevice(c3d).CreateInputLayout(iedescs.data(), (UINT)iedescs.size(), shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &INPUT_LAYOUT);
	  }
  public:
	  void Bind(const Canvas3D& c3d) const override
	  {
		  auto& Context = GetContext(c3d);
		  Context.VSSetShader(SHADER.Get(), nullptr, 0u);
		  Context.IASetInputLayout(INPUT_LAYOUT.Get());

		  if (C_BUFFER)
		  {
			  Context.VSSetConstantBuffers(0u , 1u , C_BUFFER->GetAddressOf());
		  }

	  }
};