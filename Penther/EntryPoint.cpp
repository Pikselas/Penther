#include"Window.h"
#include"Canvas2D.h"

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd("wnd" , 900 , 600);
	Canvas2D cntx { wnd };

	Image2D img(L"C:/Users/Aritra Maji/Downloads/93428795_p0.png");

	wnd.keyboard.OnKeyPress = [&](Window::KeyBoard::EventT ev) {
		if (ev.KEY_CODE == 'C')
		{
			cntx.Clear();
		}
	};

	int x_pos = 0, y_pos = 0;
	int img_xpos = 0, img_ypos = 0;

	wnd.mouse.OnRightPress = [&](Window&) {
	
		x_pos = wnd.mouse.GetX();
		y_pos = wnd.mouse.GetY();
		
	};

	wnd.mouse.OnRightRelease = [&](Window&) {
		
		img_xpos +=  wnd.mouse.GetX() - x_pos;
		img_ypos +=  wnd.mouse.GetY() - y_pos;
	};

	wnd.mouse.OnMove = [&](Window&) mutable {
		if (wnd.mouse.IsLeftPressed())
		{
			auto[x, y] = wnd.mouse.GetXY();
			const auto half = wnd.GetWidth() / 2;
			auto x1 = x < half ? half + (half - x) : half - (x - half);
			cntx.DrawPixel(x, y, { 255 , 255 , 255 });
			cntx.DrawPixel(x1, y, { 255 , 255 , 255 });
		}
		else if(wnd.mouse.IsRightPressed())
		{
			cntx.Clear();
			cntx.DrawImage(img, img_xpos + wnd.mouse.GetX() - x_pos, img_ypos + wnd.mouse.GetY() - y_pos);
		}
		
	};


	cntx.DrawImage(img);

	while (wnd.IsOpen())
	{
		wnd.ProcessEvents();
		cntx.DrawOnWindow();
	}
	return 0;
}