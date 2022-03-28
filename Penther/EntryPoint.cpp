#include"Window.h"

int WINAPI WinMain(HINSTANCE hinst , HINSTANCE hprev , LPSTR cmdln , int cmds)
{
	Window wnd(L"wnd" , 200 , 400);

	wnd.MainLoop();
	return 0;
}