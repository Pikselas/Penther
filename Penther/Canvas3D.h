#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<DirectXMath.h>

#include"Window.h"
#include"Shader.h"

class Canvas3D
{
	template<typename t>
	using PtrManager = Microsoft::WRL::ComPtr<t>;
	
	private:
		PtrManager<ID3D11Device> Device;
		PtrManager<IDXGISwapChain> SwapChain;
		PtrManager<ID3D11DeviceContext> ImmediateContext;
		PtrManager<ID3D11RenderTargetView> RenderTarget;
		PtrManager<ID3D11DepthStencilView> DepthView;
	public:
		Canvas3D(const Window& wnd);
	public:
		void SetShader(const Shader& shader) const;
	    void Draw();
};