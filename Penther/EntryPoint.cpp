#include"Window.h"
#include"Canvas2D.h"

#define Binder1(func , param) std::bind(func , param , std::placeholders::_1)
#define Binder2(func , param1 , param2)  std::bind(func , param1 , param2 , std::placeholders::_1)

template<typename funcT , typename ... argTs>
auto Binder(funcT func, argTs ... args)
{
	return std::bind(func, args... , std::placeholders::_1);
}

struct POS_T
{
	int x_pos = 0, y_pos = 0;
	int img_xpos = 0, img_ypos = 0;
};
void keyboardPress(Canvas2D& cntx , Window::KeyBoard::EventT ev)
{
	if (ev.KEY_CODE == 'C')
	{
		cntx.Clear();
	}
}
void MouseRightPress(Canvas2D& cntx, Window& wnd)
{
	POS_T* const pos_t = static_cast<POS_T*>(wnd.UserSideDataPointer);
	pos_t->x_pos = wnd.mouse.GetX();
	pos_t->y_pos = wnd.mouse.GetY();
}
void MouseRightRelease(Canvas2D& cntx, Window& wnd)
{
	POS_T* const pos_t = static_cast<POS_T*>(wnd.UserSideDataPointer);
	pos_t->img_xpos += wnd.mouse.GetX() - pos_t->x_pos;
	pos_t->img_ypos += wnd.mouse.GetY() - pos_t->y_pos;
}
void MouseMove(Canvas2D& cntx, Image2D& img , Window& wnd)
{
	POS_T* const pos_t = static_cast<POS_T*>(wnd.UserSideDataPointer);
	if (wnd.mouse.IsLeftPressed())
	{
		auto [x, y] = wnd.mouse.GetXY();
		const auto half = wnd.GetWidth() / 2;
		auto x1 = x < half ? half + (half - x) : half - (x - half);
		cntx.DrawPixel(x, y, { 255 , 255 , 255 });
		cntx.DrawPixel(x1, y, { 255 , 255 , 255 });
		cntx.DrawOnWindow();
	}
	else if (wnd.mouse.IsRightPressed())
	{
		cntx.Clear();
		cntx.DrawImage(img, pos_t->img_xpos + wnd.mouse.GetX() - pos_t->x_pos, pos_t->img_ypos + wnd.mouse.GetY() - pos_t->y_pos);
		cntx.DrawOnWindow();
	}
}

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd1("wnd1" , 500 , 700);
	Window wnd2("wnd2", 500, 700);
	Window wnd3("wnd3", 500, 700);
	
	Canvas2D cntx1 { wnd1 };
	Canvas2D cntx2 { wnd2 };
	Canvas2D cntx3 { wnd3 };

	Image2D img1(L"C:/Users/Aritra Maji/Downloads/93428795_p0.png");
	Image2D img2(L"C:/Users/Aritra Maji/Downloads/mona-finden-lovestory.jpg");
	Image2D img3(L"C:/Users/Aritra Maji/Downloads/japan/31020.jpg");

	POS_T pst1;
	POS_T pst2;
	POS_T pst3;

	wnd1.UserSideDataPointer = &pst1;
	wnd2.UserSideDataPointer = &pst2;
	wnd3.UserSideDataPointer = &pst3;

	wnd1.keyboard.OnKeyPress  =	Binder(keyboardPress , std::ref(cntx1));
	wnd1.mouse.OnRightPress   =	Binder(MouseRightPress , std::ref(cntx1));
	wnd1.mouse.OnRightRelease =	Binder(MouseRightRelease, std::ref(cntx1));
	wnd1.mouse.OnMove         =	Binder(MouseMove, std::ref(cntx1), std::ref(img1));

	wnd2.keyboard.OnKeyPress  = Binder(keyboardPress, std::ref(cntx2));
	wnd2.mouse.OnRightPress   = Binder(MouseRightPress, std::ref(cntx2));
	wnd2.mouse.OnRightRelease = Binder(MouseRightRelease, std::ref(cntx2));
	wnd2.mouse.OnMove		  = Binder(MouseMove, std::ref(cntx2), std::ref(img2));

	wnd3.keyboard.OnKeyPress  = Binder(keyboardPress, std::ref(cntx3));
	wnd3.mouse.OnRightPress   = Binder(MouseRightPress, std::ref(cntx3));
	wnd3.mouse.OnRightRelease = Binder(MouseRightRelease, std::ref(cntx3));
	wnd3.mouse.OnMove	      = Binder(MouseMove, std::ref(cntx3), std::ref(img3));

	cntx1.DrawImage(img1);
	cntx2.DrawImage(img2);
	cntx3.DrawImage(img3);
	
	cntx1.DrawOnWindow();
	cntx2.DrawOnWindow();
	cntx3.DrawOnWindow();

	while (Window::GetWindowCount())
	{
		Window::ProcessWindowEvents();
	}
	return 0;
}