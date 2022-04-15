#include"Window.h"
#include"Canvas2D.h"

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd;
	Canvas2D cntx { wnd };
	
	Image2D img(L"D:/CoderWallp/204.jpg");

	cntx.DrawImage(img);

	wnd.keyboard.EnableKeyRepeat();

	wnd.keyboard.OnKeyPress = [&](Window::KeyBoard::EventT ev) {
		if (ev.KEY_CODE == 'C')
		{
			cntx.Clear();
		}
	};

	wnd.mouse.OnMove = [&](Window&) {
		if (wnd.mouse.IsLeftPressed())
		{
			auto[x, y] = wnd.mouse.GetXY();
			const auto half = wnd.GetWidth() / 2;
			auto x1 = x < half ? half + (half - x) : half - (x - half);
			cntx.DrawPixel(x, y, { 255 , 255 , 255 });
			cntx.DrawPixel(x1, y, { 255 , 255 , 255 });
		}
	};

	wnd.keyboard.OnKeyPress = [&, x = 0, y = 0](Window::KeyBoard::EventT ev)  mutable {
		switch (ev.KEY_CODE)
		{
		case 'W':
			y -= 20;
			break;
		case 'A':
			x -= 20;
			break;
		case 'S':
			y += 20;
			break;
		case 'D':
			x += 20;
			break;
		}
		cntx.Clear();
		cntx.DrawImage(img, x, y);
	};

	while (wnd.IsOpen())
	{
		wnd.ProcessEvents();
		cntx.DrawOnWindow();
	}
	return 0;
}