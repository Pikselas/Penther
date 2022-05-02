#include "Canvas3D.h"

Canvas3D::Canvas3D(const Window& wnd) : Halfheight(wnd.height / 2) , Halfwidth(wnd.width / 2)
{

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0; 
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0; 
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferCount = 1;  
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 
	sd.Flags = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = wnd.window_handle;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, nullptr, &ImmediateContext);

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &RenderTarget);

	ImmediateContext->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), nullptr);

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)wnd.width;  
	vp.Height = (float)wnd.height; 
	vp.MaxDepth = 1; 
	vp.MinDepth = 0;
	ImmediateContext->RSSetViewports(1u, &vp);

	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Canvas3D::SetShader(const Shader& shader) const
{
	shader.Bind(*Device.Get(), *ImmediateContext.Get());
}

void Canvas3D::Draw(const ObjectBuffer& buffer)
{
	buffer.Draw(*Device.Get(), *ImmediateContext.Get());
}

void Canvas3D::ClearCanvas() const
{
	float color[] = { 1.0f , 1.0f , 1.0f , 1.0f };
	ImmediateContext->ClearRenderTargetView(RenderTarget.Get(), color);
}

void Canvas3D::PresentOnScreen() const
{
	SwapChain->Present(1u, 0u);
}

std::pair<float, float> Canvas3D::GetNormalizedWindowPos(int x, int y) const
{
	return { (x / Halfwidth ) - 1.0f,  - ((y / Halfheight ) - 1.0f)};
}
