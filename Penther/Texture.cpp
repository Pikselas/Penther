#include "Texture.h"

Texture::Texture(const Canvas3D& canvas, const Image2D& img)
{
	D3D11_TEXTURE2D_DESC tdesc = { 0 };
	tdesc.Width = img.GetWidth();
	tdesc.Height = img.GetHeight();
	tdesc.ArraySize = 1;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0u;
	tdesc.MipLevels = 1;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.MiscFlags = 0u;
	tdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	D3D11_SUBRESOURCE_DATA tsd = { 0 };
	tsd.pSysMem = img.GetRaw();
	tsd.SysMemPitch = img.GetWidth() * sizeof(ColorType);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	GetDevice(canvas).CreateTexture2D(&tdesc, &tsd, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc = {};
	srvdesc.Format = tdesc.Format;
	srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Texture2D.MipLevels = 1;
	srvdesc.Texture2D.MostDetailedMip = 0;
	GetDevice(canvas).CreateShaderResourceView(texture.Get(), &srvdesc, &TEXTURE_VIEW);

	D3D11_SAMPLER_DESC sdesc;
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GetDevice(canvas).CreateSamplerState(&sdesc, &SAMPLER);
}
