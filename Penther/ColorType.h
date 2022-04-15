#pragma once
struct ColorType
{
	unsigned char blue = 255;
	unsigned char green = 255;
	unsigned char red = 255;
	unsigned char alpha_maybe = 255;
	ColorType() = default;
	ColorType(unsigned char red, unsigned char green, unsigned char blue) : blue(blue), red(red), green(green) {}
};