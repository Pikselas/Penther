#pragma once
#include"Shader.h"
#include<vector>
#include<optional>

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

class VertexShader : public Shader
{
 private:
	 std::vector<D3D11_INPUT_ELEMENT_DESC> iedescs;
  public:
	  VertexShader(const std::wstring& cso_file , const std::vector<InputElemDesc>& descs , std::optional<const ConstBufferDesc> cbuff_desc = {}) : Shader(cbuff_desc)
	  {
		  D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
		  for (const auto& desc : descs)
		  {
			  iedescs.emplace_back(desc.SEMANTIC_NAME.c_str() , 0 , static_cast<DXGI_FORMAT>(desc.FORMAT) , 0 , desc.OFFSET , D3D11_INPUT_PER_VERTEX_DATA , 0);
		  }
	  }
  public:
	  void Bind(ID3D11Device& Device, ID3D11DeviceContext& Context) const override
	  {
		  Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		  Device.CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &shader);
		  Context.VSSetShader(shader.Get(), nullptr, 0u);

		  Microsoft::WRL::ComPtr<ID3D11InputLayout> inpl;

		  Device.CreateInputLayout(iedescs.data(), (UINT)iedescs.size(), shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &inpl);
		  Context.IASetInputLayout(inpl.Get());

		  if (CBUFF_DATA)
		  {
			  Microsoft::WRL::ComPtr<ID3D11Buffer> ConstBuff;
			  Device.CreateBuffer(&CBUFF_DESC, &CBUFF_RES, &ConstBuff);
			  Context.VSSetConstantBuffers(0u, 1u, ConstBuff.GetAddressOf());
		  }
	  }
};