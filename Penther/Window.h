#pragma once
#include<Windows.h>
#include<sstream>
#include<bitset>
#include<functional>
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
	public:
		class Mouse
		{
		friend class Window;
		private:
			int x = -1;
			int y = -1;
		private:
			bool LeftPressed = false;
			bool RightPressed = false;
		public:
			bool IsLeftPressed() const;
			bool IsRightPressed() const;
			int GetX() const;
			int GetY() const;
			std::pair<int, int> GetXY() const;
			void Reset();
		public:
			using EventHandlerType = std::function<void(Window&)>;
			EventHandlerType OnMove				= nullptr;
			EventHandlerType OnLeftPress		= nullptr;
			EventHandlerType OnRightPress		= nullptr;
			EventHandlerType OnLeftRelease		= nullptr;
			EventHandlerType OnRightRelease		= nullptr;
			EventHandlerType OnLeftDoubleClick	= nullptr;
			EventHandlerType OnRightDoubleClick = nullptr;
		};
		class KeyBoard
		{
			friend class Window;
		private:
			std::bitset<255> KEY_STAT;
			bool REPEAT_ENABLED = false;
		public:
			bool IsKeyDown(unsigned char keycode) const;
			bool IsRepeatEnabled() const;
			void EnableKeyRepeat();
			void DisableKeyRepeat();
			void Reset();
		public:
			struct EventT
			{
				Window& window;
				unsigned char KEY_CODE;
				bool IS_REPEATED;
				EventT(Window& wnd, unsigned char code, bool repeat);
			};
		public:
			using EventHandlerType = std::function<void(EventT)>;
			EventHandlerType OnKeyPress		= nullptr;
			EventHandlerType OnKeyRelease	= nullptr;
			EventHandlerType OnCharInput	= nullptr;
		};
	private:
		std::wstring name;
		int height;
		int width;
		bool Closed = false;
		static int WindowCount;
	private:
		HWND window_handle;
	private:
		static LRESULT StaticMessageHandler(HWND handle , UINT msgcode , WPARAM wparam ,LPARAM lparam);
		LRESULT MessageHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam);
	public:
		Mouse mouse;
		KeyBoard keyboard;
	public:
		Window();
		Window(const std::wstring& name, int height, int width);
		void ChangeTitle(const std::wstring& title);
		void ProcessEvents() const;
	public:
		constexpr static Window* ALL_WINDOWS = nullptr;
	public:
		static void MainLoop(const Window* const window);
};