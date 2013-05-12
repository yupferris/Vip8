#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Common.h"
#include "List.h"

class Window
{
public:
	typedef void (* EventHandler)(Window& w, int event, WPARAM wParam, LPARAM lParam);

	Window();
	Window(string title, bool fullscreen = false, int w = 800, int h = 600, int style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	~Window();
	virtual void Open(string title, bool fullscreen = false, int w = 800, int h = 600, int style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	virtual void Close();
	virtual void Update();
	virtual void ClearEventHandlers();
	virtual void AddEventHandler(EventHandler eh);
	virtual void RaiseEvent(int Event, WPARAM wParam, LPARAM lParam);
	virtual void SetTitle(string title);
	virtual void SetSize(int w, int h);
	virtual void SetMenu(HMENU menu);
	virtual bool IsOpen() const;
	virtual HWND GetHandle() const;
	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual float GetAspectRatio() const;

protected:
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProcRouter(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static void defaultEventHandler(Window& w, int Event, WPARAM wParam, LPARAM lParam);
	bool open;
	HWND handle;
	HMENU menu;
	HINSTANCE instance;
	int x, y;
	int width, height;
	int style;
	bool fullscreen;
	List<EventHandler> eventHandlers;
};

#endif
