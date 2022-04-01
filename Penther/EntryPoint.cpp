#include"Window.h"
#include<sstream>
#include<bitset>

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd1;
	Window::MainLoop(Window::ALL_WINDOWS);
	return 0;
}