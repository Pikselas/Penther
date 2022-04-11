#pragma once
#include<d3d11.h>
#include<wrl.h>
#include"Window.h"
class Canvas2D
{
private:
	unsigned int height;
	unsigned int width;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> ImmediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
public:
	using Exception = Window::Exception;
public:
	Canvas2D(const Window& window);
};