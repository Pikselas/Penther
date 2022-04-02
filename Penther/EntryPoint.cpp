#include"Window.h"
#include<sstream>
#include<bitset>

void press(Window::KeyBoard::EventT ev)
{
	static std::wstringstream ws;
	ws << (char)ev.KEY_CODE;
	ev.window.ChangeTitle(ws.str());
}

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd;

	wnd.keyboard.OnKeyRelease = press;

	Window::MainLoop(Window::ALL_WINDOWS);
	return 0;
}