#pragma once
#include<memory>
#include<string>
#include"GDIPlusManager.h"
#include"ColorType.h"

class Canvas2D;

class Image2D
{
private:
	size_t height = 0 ;
	size_t width = 0;
	std::unique_ptr<ColorType[]> PIXEL_DATA = nullptr;
public:
	Image2D(const std::wstring& file);
	size_t GetHeight() const;
	size_t GetWidth() const;
	ColorType GetPixel(unsigned int x , unsigned int y) const;
	const ColorType* GetRaw() const;
};