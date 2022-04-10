#include"Window.h"
#include<thread>

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	try
	{
		Window wnd;

		while (wnd.IsOpen())
		{
			wnd.ProcessEvents();
		}
	}
	catch (Window::Exception e)
	{
		std::stringstream ss;
		ss << "[[FILE]]" << " " << e.GetFile() << "\n"
			<< "[[LINE]]" << " " << e.GetLine() << "\n"
			<< "[[REASON]]" << " " << e.GetReason();
		MessageBox(nullptr, ss.str().c_str(), "Window error", MB_ICONERROR);
	}
	return 0;
}