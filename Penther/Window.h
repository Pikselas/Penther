#pragma once
#include<Windows.h>
#include<sstream>
#include<bitset>
#include<functional>
#include<source_location>
#include"resource.h"
class Window
{
	private:
		class WindowClass
		{
		 private:
			constexpr static const char* classNm = "PentherWindowClass";
			WindowClass();
			~WindowClass();
			static WindowClass wndcls;
			HINSTANCE hinst;
		 public:
			 constexpr static const char* GetName();
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
		class Exception
		{
		private:
			const HRESULT code;
			std::string message;
			std::string file;
			unsigned int line;
		public:
			Exception(HRESULT hr , std::source_location s = std::source_location::current());
			std::string GetReason() const;
			unsigned int GetLine() const;
			std::string GetFile() const;
			HRESULT GetErrorCode() const;
		};
	private:
		std::string name;
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
		Window(const std::string& name, int height, int width);
		~Window();
		bool IsOpen() const;
		void ChangeTitle(const std::string& title);
		void ProcessEvents() const;
	public:
		constexpr static Window* ALL_WINDOWS = nullptr;
	public:
		static void MainLoop(const Window* const window);
		static void ProcessWindowEvents();
		static int GetWindowCount();
};

#define THROW_IF_FAILED(hrcall) if(HRESULT hrcode = hr ; FAILED(hrcode)) throw Window::Exception(hrcode)