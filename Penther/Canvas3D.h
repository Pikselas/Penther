#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<DirectXMath.h>

#include"Window.h"
#include"Shader.h"
#include"ObjectBuffer.h"

template<class CanvasType>
class CanvasAccesser;

class Canvas3D
{
	template<typename t>
	using PtrManager = Microsoft::WRL::ComPtr<t>;
	using DrawableObject = ObjectBuffer<Canvas3D>;
	friend ObjectBuffer<Canvas3D>;
	friend CanvasAccesser<Canvas3D>;
	private:
		PtrManager<ID3D11Device> Device;
		PtrManager<IDXGISwapChain> SwapChain;
		PtrManager<ID3D11DeviceContext> ImmediateContext;
		PtrManager<ID3D11RenderTargetView> RenderTarget;
		PtrManager<ID3D11DepthStencilView> DepthView;
	private:
		const float Halfheight;
		const float Halfwidth;
	public:
		Canvas3D(const Window& wnd);
	public:
		std::pair<float, float> GetNormalizedWindowPos(int x , int y) const;
		void SetShader(const Shader& shader) const;
		void ClearCanvas() const;
		void PresentOnScreen() const;
	    void Draw(const DrawableObject& buffer);
};