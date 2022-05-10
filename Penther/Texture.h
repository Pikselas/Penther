#pragma once
#include"Canvas3D.h"
#include"CanvasAccesser.h"
#include"Image2D.h"

class PixelShader;

class Texture : CanvasAccesser
{
	friend PixelShader;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TEXTURE_VIEW;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SAMPLER;
public:
	Texture(const Canvas3D& canvas, const Image2D& img);
};