#include "Canvas2D.h"

Canvas2D::Canvas2D(const Window& window) : height(window.height) , width(window.width)
{
	using namespace Microsoft::WRL;
	
	//description for creating swapchain , device and device context
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.OutputWindow = window.window_handle;
	sd.Flags = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//creating swapchain , device and device context
	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &sd, &SwapChain, &Device,
		nullptr, &ImmediateContext));
	
	//get handle of back buuffer of swapchain
	ComPtr<ID3D11Resource> pBackBuffer;
	THROW_IF_FAILED(SwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer));

	// create render target view from (as??) back buffer of swapchain 
	THROW_IF_FAILED(Device->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&RenderTargetView));

	ImmediateContext->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), nullptr);

	//creating viewport
	D3D11_VIEWPORT vp;
	vp.Width = float(width);
	vp.Height = float(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	ImmediateContext->RSSetViewports(1, &vp);

	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = width;
	sysTexDesc.Height = height;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	THROW_IF_FAILED(Device->CreateTexture2D(&sysTexDesc, nullptr, &SysBufferTexture));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	// create the resource view on the texture
	THROW_IF_FAILED(Device->CreateShaderResourceView(SysBufferTexture.Get(),
		&srvDesc, &SysBufferTextureView));

	ComPtr<ID3DBlob> pBlob;

	D3DReadFileToBlob(L"Canvas2DPixelShader.cso", &pBlob);

	// create pixel shader for framebuffer
	THROW_IF_FAILED(Device->CreatePixelShader(
		pBlob->GetBufferPointer() , pBlob->GetBufferSize() , nullptr , &PixelShader));

	D3DReadFileToBlob(L"Canvas2DVertexShader.cso", &pBlob);

	// create vertex shader for framebuffer
	THROW_IF_FAILED(Device->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&VertexShader));

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FSQVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	THROW_IF_FAILED(Device->CreateBuffer(&bd, &initData, &VertexBuffer));


	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	THROW_IF_FAILED(Device->CreateInputLayout(ied, 2,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&InputLayout));


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	THROW_IF_FAILED(Device->CreateSamplerState(&sampDesc, &SamplerState));

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	const size_t buffsize = sizeof(ColorT) * width * height;
	PixelData = reinterpret_cast<ColorT*>(
		_aligned_malloc(buffsize, 16u));
	if (PixelData)
	{
		memset(PixelData, 0, buffsize);
	}
	else
	{
		throw std::runtime_error("Error Allocating memory");
	}
}

Canvas2D::~Canvas2D()
{
	if (PixelData)
	{
		_aligned_free(PixelData);
	}
}

void Canvas2D::DrawPixel(unsigned x, unsigned y, ColorT color) const
{
	PixelData[y * width + x] = color;
}

void Canvas2D::DrawImage(const std::wstring& file, int x, int y) const
{
	GDIPlusManager manager;
	Gdiplus::Bitmap bitmap(file.c_str());
	if (bitmap.GetLastStatus() == Gdiplus::Status::Ok)
	{
		Gdiplus::Color c;
		const auto Imgheight = bitmap.GetHeight();
		const auto Imgwidth = bitmap.GetWidth();

		unsigned int imgX = 0 , screenX = 0;
		unsigned int imgY = 0 , screenY = 0;

		(x < 0 ? imgX : screenX) = abs(x);
		(y < 0 ? imgY : screenY) = abs(y);

		for ( ; imgY < Imgheight && screenY < height; ++imgY, ++screenY)
		{
			for (auto i = imgX, j = screenX; i < Imgwidth && j < width; ++i, ++j)
			{
				bitmap.GetPixel(i, imgY, &c);
				DrawPixel(j, screenY, { c.GetB() , c.GetG() , c.GetR() });
			}
		}
	}
}

void Canvas2D::Clear()
{
	memset(PixelData, 0,(sizeof(ColorT)) * width * height);
}

void Canvas2D::DrawOnWindow()
{
	D3D11_MAPPED_SUBRESOURCE mappedSysBufferTexture;
	THROW_IF_FAILED(ImmediateContext->Map(SysBufferTexture.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture));
	
	// setup parameters for copy operation
	ColorT* pDst = reinterpret_cast<ColorT*>(mappedSysBufferTexture.pData);
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof(ColorT);
	const size_t srcPitch = width;
	const size_t rowBytes = srcPitch * sizeof(ColorT);
	// perform the copy line-by-line
	for (size_t y = 0u; y < height; y++)
	{
		memcpy(&pDst[y * dstPitch], &PixelData[y * srcPitch], rowBytes);
	}
	// release the adapter memory
	ImmediateContext->Unmap(SysBufferTexture.Get(), 0u);

	// render offscreen scene texture to back buffer
	ImmediateContext->IASetInputLayout(InputLayout.Get());
	ImmediateContext->VSSetShader(VertexShader.Get(), nullptr, 0u);
	ImmediateContext->PSSetShader(PixelShader.Get(), nullptr, 0u);
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const UINT stride = sizeof(FSQVertex);
	const UINT offset = 0u;
	ImmediateContext->IASetVertexBuffers(0u, 1u, VertexBuffer.GetAddressOf(), &stride, &offset);
	ImmediateContext->PSSetShaderResources(0u, 1u, SysBufferTextureView.GetAddressOf());
	ImmediateContext->PSSetSamplers(0u, 1u, SamplerState.GetAddressOf());
	ImmediateContext->Draw(6u, 0u);

	// flip back/front buffers
	THROW_IF_FAILED(SwapChain->Present(1u, 0u));
}
