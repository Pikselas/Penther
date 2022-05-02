#include "Canvas3D.h"

Canvas3D::Canvas3D(const Window& wnd)
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

	struct VertexType
	{
		float x, y, z;
	};

	VertexType vertices[] = {
		{0.0 , 0.5 , 0.0},
		{0.5 , -0.5 , 0.0},
		{-0.5 , -0.5 , 0.0}
	};

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(vertices);					
	bd.Usage = D3D11_USAGE_DEFAULT;				
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
	bd.CPUAccessFlags = 0u;							
	bd.MiscFlags = 0u;							
	bd.StructureByteStride = sizeof(VertexType); 

	
	D3D11_SUBRESOURCE_DATA subd = { 0 };
	subd.pSysMem = vertices;							

	Microsoft::WRL::ComPtr<ID3D11Buffer> VBuffer;
	Device->CreateBuffer(&bd, &subd, &VBuffer);
	UINT stride = sizeof(VertexType);			
	UINT offset = 0u;							
	
	ImmediateContext->IASetVertexBuffers(0u, 1u, VBuffer.GetAddressOf(), &stride, &offset);

	ImmediateContext->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), nullptr);

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = wnd.width;  
	vp.Height = wnd.height; 
	vp.MaxDepth = 1; 
	vp.MinDepth = 0;
	ImmediateContext->RSSetViewports(1u, &vp);

	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Canvas3D::SetShader(const Shader& shader) const
{
	shader.Bind(*Device.Get(), *ImmediateContext.Get());
}

void Canvas3D::Draw()
{
	ImmediateContext->Draw(3u, 0u);
	SwapChain->Present(1u , 0u);
}
