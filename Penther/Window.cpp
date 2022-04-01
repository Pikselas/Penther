#include"Window.h"

Window::WindowClass Window::WindowClass::wndcls;

Window::WindowClass::WindowClass()
{
	hinst = GetModuleHandle(nullptr);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = hinst;
	wc.lpszClassName = classNm;
	wc.style = CS_DBLCLKS; //Enables window to take double click events
	wc.lpfnWndProc = StaticMessageHandler;
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(classNm , hinst);
}

constexpr const wchar_t* Window::WindowClass::GetName()
{
	return classNm;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return wndcls.hinst;
}

int Window::WindowCount = 0;

LRESULT Window::StaticMessageHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	Window * const wnd_ptr = reinterpret_cast<Window*>(GetWindowLongPtr(handle , GWLP_USERDATA));
	return wnd_ptr->MessageHandler(handle , msgcode , wparam , lparam);
}

LRESULT Window::MessageHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lparam);
		mouse.x = pt.x;
		mouse.y = pt.y;
	}
	}
	switch (msgcode)
	{
	 case WM_CLOSE:
		 Closed = true;
		 --WindowCount;
		 break;
	 case WM_MOUSEMOVE:
		 if (mouse.OnMove)
		 {
			 mouse.OnMove(*this);
		 }
		 break;
	 case WM_LBUTTONUP:
		 mouse.LeftPressed = false;
		 if (mouse.OnLeftRelease)
		 {
			 mouse.OnLeftRelease(*this);
		 }
	 break;
	 case WM_LBUTTONDOWN:
		 mouse.LeftPressed = true;
		 if (mouse.OnLeftPress)
		 {
			 mouse.OnLeftPress(*this);
		 }
	 break;
	 case WM_LBUTTONDBLCLK:
		 if (mouse.OnLeftDoubleClick)
		 {
			 mouse.OnLeftDoubleClick(*this);
		}
	 break;
	 case WM_RBUTTONUP:
		 mouse.RightPressed = false;
		 if (mouse.OnRightRelease)
		 {
			 mouse.OnRightRelease(*this);
		 }
	 break;
	 case WM_RBUTTONDOWN:
		 mouse.RightPressed = true;
		 if (mouse.OnRightPress)
		 {
			 mouse.OnRightPress(*this);
		 }
	 break;
	 case WM_RBUTTONDBLCLK:
		 if (mouse.OnRightDoubleClick)
		 {
			 mouse.OnRightDoubleClick(*this);
		 }
	 break;
	 case WM_KEYDOWN:
	 {
		 static std::wstring ws;
		 ws.push_back('H');
		 SetWindowText(handle, ws.c_str());
	 }
	 break;
	}
	return DefWindowProc(handle , msgcode , wparam , lparam);
}

Window::Window() : Window(L"Window", 600 , 900 ){}

Window::Window(const std::wstring& name, int height, int width) : name(name), height(height), width(width) , mouse(*this)
{
	window_handle = CreateWindowEx(0, WindowClass::GetName(), name.c_str(), WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr,
		WindowClass::GetInstance(), nullptr);
	SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	++WindowCount;
}

void Window::ChangeTitle(const std::wstring& title)
{
	SetWindowText(window_handle, title.c_str());
}

void Window::ProcessEvents() const
{
	Window::MainLoop(this);
}

void Window::MainLoop(const Window* const window)
{
	MSG msg;
	if (window != nullptr)
	{
		while (true)
		{
			PeekMessage(&msg, window->window_handle, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (window->Closed)
			{
				break;
			}
		}
	}
	else
	{
		while (WindowCount > 0)
		{
			PeekMessage(&msg, nullptr , 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

bool Window::Mouse::IsLeftPressed() const
{
	return LeftPressed;
}

bool Window::Mouse::IsRightPressed() const
{
	return RightPressed;
}

int Window::Mouse::GetX() const
{
	return x;
}

int Window::Mouse::GetY() const
{
	return y;
}

std::pair<int, int> Window::Mouse::GetXY() const
{
	return {x , y};
}
