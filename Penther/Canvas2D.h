#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include"Window.h"
#include"GDIPlusManager.h"

class Canvas2D
{
private:
	unsigned int height;
	unsigned int width;
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		ImmediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			SysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>SysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		PixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		VertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		InputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	    SamplerState;
public:
	class Exception :public Window::Exception {};
	struct ColorT
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		unsigned char alpha_maybe;
	};
	struct FSQVertex
	{
		float x, y, z;		// position
		float u, v;			// texcoords
	};
private:
	ColorT * PixelData = nullptr;
public:
	Canvas2D(const Window& window);
	~Canvas2D();
public:
	void DrawPixel(unsigned x , unsigned y , ColorT color) const;
	void DrawImage(const std::wstring& file , unsigned int x = 0 ,unsigned int y = 0) const;
	void Clear();
	void DrawOnWindow();
};