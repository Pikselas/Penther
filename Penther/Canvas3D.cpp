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

	D3D11_DEPTH_STENCIL_DESC depthDesc = { 0 };
	depthDesc.DepthEnable = TRUE; 
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS; 

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencil;
	Device->CreateDepthStencilState(&depthDesc, &pDepthStencil);

	ImmediateContext->OMSetDepthStencilState(pDepthStencil.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthTexture;
	D3D11_TEXTURE2D_DESC depthTexDesc = { 0 };
	depthTexDesc.Height = wnd.height;
	depthTexDesc.Width = wnd.width;
	depthTexDesc.MipLevels = 1u; 
	depthTexDesc.ArraySize = 1u; 
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT; 
	depthTexDesc.SampleDesc.Count = 1u;
	depthTexDesc.SampleDesc.Quality = 0u;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;

	Device->CreateTexture2D(&depthTexDesc, nullptr, &pDepthTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {  };
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0u;
	
	Device->CreateDepthStencilView(pDepthTexture.Get(), &depthViewDesc, &DepthView);

	ImmediateContext->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), DepthView.Get());

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
	shader.Bind(*ImmediateContext.Get());
}

void Canvas3D::Draw(const DrawableObject& buffer)
{
	buffer.Draw(*ImmediateContext.Get());
}

void Canvas3D::ClearCanvas() const
{
	float color[] = { 0.0f , 0.0f , 0.0f , 0.0f };
	ImmediateContext->ClearRenderTargetView(RenderTarget.Get(), color);
	ImmediateContext->ClearDepthStencilView(DepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Canvas3D::PresentOnScreen() const
{
	SwapChain->Present(1u, 0u);
}

std::pair<float, float> Canvas3D::GetNormalizedWindowPos(int x, int y) const
{
	return { (x / Halfwidth ) - 1.0f,  - ((y / Halfheight ) - 1.0f)};
}
