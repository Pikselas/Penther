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
	wc.hIcon = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1) , IMAGE_ICON,256 , 256 , 0 ));
	wc.hIconSm = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 256 , 256 , 0));
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(classNm , hinst);
}

constexpr const char* Window::WindowClass::GetName()
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
		 //if repeated then bit 30 of lparam will be true
		 constexpr unsigned int Mask = 0b1 << 30;
		 bool repeated = ((Mask & lparam) != 0);
		 if (!repeated || keyboard.IsRepeatEnabled())
		 {
			 keyboard.KEY_STAT[static_cast<unsigned char>(wparam)] = true;
			 if (keyboard.OnKeyPress)
			 {
				 keyboard.OnKeyPress({*this , static_cast<unsigned char>(wparam)  , repeated});
			 }
		 }
	 }
	 break;
	 case WM_KEYUP:
	 {
		 keyboard.KEY_STAT[static_cast<unsigned char>(wparam)] = false;
		 if (keyboard.OnKeyRelease)
		 {
		   keyboard.OnKeyRelease({ *this , static_cast<unsigned char>(wparam)  , false });
		 }
	 }
	 break;
	 case WM_CHAR:
	 {
		 constexpr unsigned int Mask = 0b1 << 30;
		 bool repeated = ((Mask & lparam) != 0);
		 if (keyboard.OnCharInput)
		 {
			 keyboard.OnCharInput({ *this , static_cast<unsigned char>(wparam) , repeated });
		 }
	 }
	 break;
	 case WM_KILLFOCUS:
		 mouse.Reset();
		 keyboard.Reset();
	}
	return DefWindowProc(handle , msgcode , wparam , lparam);
}

Window::Window() : Window("Window", 600 , 900 ){}

Window::Window(const std::string& name, int height, int width) : name(name), height(height), width(width)
{

	RECT wr = {0};
	wr.right = width;
	wr.bottom = height;

	AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, FALSE);

	window_handle = CreateWindowEx(0, WindowClass::GetName(), name.c_str(), WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr,
		WindowClass::GetInstance(), nullptr);

	if (window_handle == nullptr)
	{
		throw Exception(GetLastError());
	}
	else
	{
		SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		++WindowCount;
	}
}

Window::~Window()
{
	if (!Closed)
	{
		DestroyWindow(window_handle);
		Closed = true;
		--WindowCount;
	}
}

bool Window::IsOpen() const
{
	return !Closed;
}

void Window::ChangeTitle(const std::string& title)
{
	SetWindowText(window_handle, title.c_str());
}

void Window::ProcessEvents() const
{
	MSG msg;
	if (PeekMessage(&msg, window_handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::MainLoop(const Window* const window)
{
	MSG msg;
	if (window != nullptr)
	{
		while (!window->Closed)
		{
			if (PeekMessage(&msg, window->window_handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	else
	{
		while (WindowCount > 0)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

void Window::ProcessWindowEvents()
{
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int Window::GetWindowCount()
{
	return WindowCount;
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

void Window::Mouse::Reset()
{
	x = y = -1;
	LeftPressed = RightPressed = false;
}

bool Window::KeyBoard::IsKeyDown(unsigned char keycode) const
{
	return KEY_STAT[keycode];
}

bool Window::KeyBoard::IsRepeatEnabled() const
{
	return REPEAT_ENABLED;
}

void Window::KeyBoard::EnableKeyRepeat()
{
	REPEAT_ENABLED = true;
}

void Window::KeyBoard::DisableKeyRepeat()
{
	REPEAT_ENABLED = false;
}

void Window::KeyBoard::Reset()
{
	KEY_STAT.reset();
}

Window::KeyBoard::EventT::EventT(Window& wnd, unsigned char code, bool repeat) : window(wnd) , KEY_CODE(code) , IS_REPEATED(repeat)
{}

Window::Exception::Exception(HRESULT hr , std::source_location s) : code(hr)
{
	char * pcharBuff = nullptr;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pcharBuff), 0, nullptr);
	message = pcharBuff;
	file = s.file_name();
	line = s.line();
	LocalFree(pcharBuff);
}

std::string Window::Exception::GetReason() const
{
	return message;
}

unsigned int Window::Exception::GetLine() const
{
	return line;
}

std::string Window::Exception::GetFile() const
{
	return file;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return code;
}
