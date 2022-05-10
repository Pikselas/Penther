#pragma once
#include"Canvas3D.h"
#include"CanvasAccesser.h"

class ConstantBuffer : CanvasAccesser
{
	friend class Shader;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> BUFFER;
	ID3D11DeviceContext& ImmediateContext;
public:
	ConstantBuffer(const Canvas3D& c3d, void* MEM_LOCATION, unsigned int LENGTH_IN_BYTES) : ImmediateContext(GetDeviceContext(c3d))
	{
		D3D11_BUFFER_DESC CBUFF_DESC = { 0 };
		D3D11_SUBRESOURCE_DATA CBUFF_RES = { 0 };
		CBUFF_DESC.ByteWidth = LENGTH_IN_BYTES;
		CBUFF_DESC.Usage = D3D11_USAGE_DYNAMIC;
		CBUFF_DESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBUFF_DESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		CBUFF_RES.pSysMem = MEM_LOCATION;

		GetDevice(c3d).CreateBuffer(&CBUFF_DESC, &CBUFF_RES, &BUFFER);
	}
	void Update(void* MEM_LOCATION, unsigned int LENGTH_IN_BYTES)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		ImmediateContext.Map(BUFFER.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
		std::memcpy(ms.pData, MEM_LOCATION, LENGTH_IN_BYTES);
		ImmediateContext.Unmap(BUFFER.Get(), 0u);
	}
};