#include"Window.h"

Window::WindowClass Window::WindowClass::wndcls;

Window::WindowClass::WindowClass()
{
	hinst = GetModuleHandle(nullptr);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = hinst;
	wc.lpszClassName = classNm;
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

LRESULT Window::StaticMessageHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	Window * const wnd_ptr = reinterpret_cast<Window*>(GetWindowLongPtr(handle , GWLP_USERDATA));
	return wnd_ptr->MessageHandler(handle , msgcode , wparam , lparam);
}

LRESULT Window::MessageHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
	 case WM_CLOSE:
		 Closed = true;
		 break;
	}
	return DefWindowProc(handle , msgcode , wparam , lparam);
}

Window::Window(const std::wstring& name, int height, int width) : name(name) , height(height) , width(width)
{
	window_handle = CreateWindowEx(0, WindowClass::GetName(), name.c_str(), WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr,
									WindowClass::GetInstance(), nullptr);
	SetWindowLongPtr(window_handle , GWLP_USERDATA , reinterpret_cast<LONG_PTR>(this));
}

void Window::MainLoop()
{
	MSG msg;
	while (true)
	{
		PeekMessage(&msg, window_handle , 0, 0 , PM_REMOVE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (Closed)
		{
			break;
		}
	}
}
