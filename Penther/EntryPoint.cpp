#include"Window.h"
#include"Canvas2D.h"

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	try
	{
		Window wnd;
		Canvas2D c2d(wnd);

		while (wnd.IsOpen())
		{
			if (wnd.mouse.IsLeftPressed())
			{
				const auto [x, y]  = wnd.mouse.GetXY();
				for (int i = 0 ;i < 10; ++i)
				{
					for (int j = 0; j < 10; j++)
					{
						c2d.DrawPixel(x + j , y + i , {255 , 255 , 255});
					}
				}
			}
			c2d.DrawOnWindow();
			wnd.ProcessEvents();
		}
	}
	catch (Canvas2D::Exception e)
	{
		std::stringstream ss;
		ss << "[[FILE]]" << " " << e.GetFile() << "\n"
			<< "[[LINE]]" << " " << e.GetLine() << "\n"
			<< "[[REASON]]" << " " << e.GetReason();
		MessageBox(nullptr, ss.str().c_str(), "Window error", MB_ICONERROR);
	}
	catch (Window::Exception e)
	{
		std::stringstream ss;
		ss << "[[FILE]]" << " " << e.GetFile() << "\n"
			<< "[[LINE]]" << " " << e.GetLine() << "\n"
			<< "[[REASON]]" << " " << e.GetReason();
		MessageBox(nullptr, ss.str().c_str(), "Window error", MB_ICONERROR);
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, e.what() , "standard error", MB_ICONERROR);
	}
	return 0;
}