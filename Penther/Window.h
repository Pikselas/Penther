#pragma once
#include<Windows.h>
#include<string>
class Window
{
	private:
		class WindowClass
		{
		 private:
			constexpr static const wchar_t* classNm = L"PentherWindowClass";
			WindowClass();
			~WindowClass();
			static WindowClass wndcls;
			HINSTANCE hinst;
		 public:
			 constexpr static const wchar_t* GetName();
			 static HINSTANCE GetInstance();
		};
	private:
		std::wstring name;
		int height;
		int width;
		bool Closed = false;
	private:
		HWND window_handle;
	private:
		static LRESULT StaticMessageHandler(HWND handle , UINT msgcode , WPARAM wparam ,LPARAM lparam);
		LRESULT MessageHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam);
	public:
		Window(const std::wstring& name, int height, int width);
		void MainLoop();
};