#pragma once
#include<d3d11.h>

template<class Canvas3D>
class CanvasAccesser
{
protected:
	static ID3D11Device& GetDevice(const Canvas3D& canvas)
	{
		return *canvas.Device.Get();
	}
	static ID3D11DeviceContext& GetDeviceContext(const Canvas3D& canvas)
	{
		return *canvas.ImmediateContext.Get();
	}
};