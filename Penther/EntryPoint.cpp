#include"Window.h"
#include<sstream>

void SetMouseTitle(Window::Mouse& ms)
{
	std::wstringstream s;
	s << ms.GetX() << ms.GetY();
	ms.ParentWindow.ChangeTitle(s.str());
}

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd1,wnd2;
	wnd1.mouse.OnLeftDoubleClick = SetMouseTitle;
	wnd2.mouse.OnLeftDoubleClick = SetMouseTitle;
	Window::MainLoop(Window::ALL_WINDOWS);
	return 0;
}