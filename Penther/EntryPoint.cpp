#include"Window.h"
#include"Canvas3D.h"
#include"PixelShader.h"

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd;
	Canvas3D c3d(wnd);

	c3d.SetShader(PixelShader{L"PixelShader.cso"});

	while (Window::GetWindowCount())
	{
		Window::ProcessWindowEvents();
		c3d.Draw();
	}
	return 0;
}