#include"Window.h"
#include"Canvas2D.h"

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd;
	Canvas2D cntx { wnd };
	
	cntx.DrawImage(L"D:/CoderWallp/204.jpg" , 100 , 100);


	wnd.keyboard.OnKeyPress = [&](Window::KeyBoard::EventT ev) {
		if (ev.KEY_CODE == 'W')
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

	while (wnd.IsOpen())
	{
		wnd.ProcessEvents();
		cntx.DrawOnWindow();
	}
	return 0;
}