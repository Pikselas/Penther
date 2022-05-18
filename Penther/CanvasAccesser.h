#pragma once
#include<d3d11.h>
#include<concepts>

class Canvas3D;

template<class Type>
concept CanvasType = std::is_same_v<Type, Canvas3D>;

class CanvasAccesser
{
protected:
	static ID3D11Device& GetDevice(const CanvasType auto& canvas)
	{
		return *canvas.Device.Get();
	}
	static ID3D11DeviceContext& GetDeviceContext(const CanvasType auto& canvas)
	{
		return *canvas.ImmediateContext.Get();
	}
};