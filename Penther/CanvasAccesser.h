#pragma once
#include<d3d11.h>

class CanvasAccesser
{
protected:
	static ID3D11Device& GetDevice(const auto& canvas)
	{
		return *canvas.Device.Get();
	}
	static ID3D11DeviceContext& GetDeviceContext(const auto& canvas)
	{
		return *canvas.ImmediateContext.Get();
	}
};