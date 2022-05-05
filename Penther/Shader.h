#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include<string>
#include<memory>
#include<optional>

template<typename Canvas3D>
class CBuffer;

template<typename Canvas3D>
class Shader
{
 protected:
	 std::optional<Microsoft::WRL::ComPtr<ID3D11Buffer>> C_BUFFER;
 public:
	 virtual void Bind(const Canvas3D&) const = 0;
	 Shader(std::optional<CBuffer<Canvas3D>> c_buffer)
	 {
		 if (c_buffer)
		 {
			 C_BUFFER = c_buffer->BUFFER;
		 }
	 }
 protected:
	 static ID3D11Device& GetDevice(const Canvas3D& canvas)
	 {
		 return *(canvas.Device.Get());
	 }
	 static ID3D11DeviceContext& GetContext(const Canvas3D& canvas)
	 {
		 return *(canvas.ImmediateContext.Get());
	 }
};

template<class Canvas3D>
class CBuffer
{
	friend Shader<Canvas3D>;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> BUFFER;
	const Canvas3D& canvas;
public:
	CBuffer(const Canvas3D& c3d, void* MEM_LOCATION, unsigned int LENGTH_IN_BYTES) : canvas(c3d)
	{
		D3D11_BUFFER_DESC CBUFF_DESC = { 0 };
		D3D11_SUBRESOURCE_DATA CBUFF_RES = { 0 };
		CBUFF_DESC.ByteWidth = LENGTH_IN_BYTES;
		CBUFF_DESC.Usage = D3D11_USAGE_DYNAMIC;
		CBUFF_DESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBUFF_DESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		CBUFF_RES.pSysMem = MEM_LOCATION;

		c3d.Device->CreateBuffer(&CBUFF_DESC, &CBUFF_RES, &BUFFER);
	}
	void Update(void* MEM_LOCATION, unsigned int LENGTH_IN_BYTES)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		canvas.ImmediateContext->Map(BUFFER.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
		std::memcpy(ms.pData, MEM_LOCATION, LENGTH_IN_BYTES);
		canvas.ImmediateContext->Unmap(BUFFER.Get(), 0u);
	}
};
