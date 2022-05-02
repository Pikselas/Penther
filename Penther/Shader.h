#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include<string>
#include<memory>
#include<optional>
class ConstBufferDesc
{
public:
	void* MEM_LOCATION;
	unsigned int LENGTH_IN_BYTES;
};

class Shader
{
 protected:
	 Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
	 std::unique_ptr<BYTE[]> CBUFF_DATA;
	 D3D11_BUFFER_DESC CBUFF_DESC;
	 D3D11_SUBRESOURCE_DATA CBUFF_RES;
public:
	Shader(std::optional<const ConstBufferDesc> cbuff_desc = {})
	{
		if (cbuff_desc)
		{
			CBUFF_DESC = { 0 };
			CBUFF_RES = { 0 };
			CBUFF_DESC.ByteWidth = cbuff_desc->LENGTH_IN_BYTES;
			CBUFF_DESC.Usage = D3D11_USAGE_DYNAMIC;  
			CBUFF_DESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			CBUFF_DESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			CBUFF_DATA = std::make_unique<BYTE[]>(cbuff_desc->LENGTH_IN_BYTES);
			std::memcpy(CBUFF_DATA.get(), cbuff_desc->MEM_LOCATION, cbuff_desc->LENGTH_IN_BYTES);

			CBUFF_RES.pSysMem = CBUFF_DATA.get();
		}
	}
 public:
	 virtual void Bind(ID3D11Device& Device, ID3D11DeviceContext& Context) const = 0;
};
