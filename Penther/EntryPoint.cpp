#include"Window.h"
#include<sstream>
int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd;

	wnd.ProcessEvents();
	return 0;
}