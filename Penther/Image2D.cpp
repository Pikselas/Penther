#include "Image2D.h"

Image2D::Image2D(const std::wstring& file)
{
	GDIPlusManager manager;
	Gdiplus::Bitmap bitmap(file.c_str());
	if (bitmap.GetLastStatus() == Gdiplus::Status::Ok)
	{
		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
		PIXEL_DATA = std::make_unique<ColorType[]>(height * width);
		Gdiplus::Color c;
		for (auto i = 0; i < height; ++i)
		{
			for (auto j = 0; j < width; ++j)
			{
				bitmap.GetPixel(j, i, &c);
				const auto indx = width * i + j;
				PIXEL_DATA[width * i + j] = { c.GetR() , c.GetG() , c.GetB() };
			}
		}
	}
}

size_t Image2D::GetHeight() const
{
    return height;
}

size_t Image2D::GetWidth() const 
{
    return width;
}

ColorType Image2D::GetPixel(unsigned int x, unsigned int y) const
{
	return PIXEL_DATA[width * y + x];
}

void Image2D::SetPixel(unsigned int x, unsigned int y, ColorType color)
{
	PIXEL_DATA[width * y + x] = color;
}

const ColorType* Image2D::GetRaw() const
{
	return PIXEL_DATA.get();
}
