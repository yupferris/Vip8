#include "Window.h"

#include "resource.h"

const string ClassName = "Class";

Window::Window()
{
	open = false;
}

Window::Window(string title, bool fullscreen, int w, int h, int style)
{
	Open(title, fullscreen, w, h, style);
}

Window::~Window()
{
	Close();
}

void Window::Open(string title, bool fullscreen, int w, int h, int style)
{
	menu = 0;
	instance = GetModuleHandle(NULL);
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	this->style = style;
	this->fullscreen = fullscreen;

	AddEventHandler(defaultEventHandler);

	WNDCLASSEX wndClass =
	{
		sizeof(WNDCLASSEX),
		0,
		WndProcRouter,
		0,
		0,
		instance,
		LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON1)/*NULL, IDI_APPLICATION*/),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) COLOR_WINDOW,
		NULL,
		ClassName.c_str(),
		NULL
	};

	if (!RegisterClassEx(&wndClass)) throw EXCEPTION("Could not register window class.");
	if (fullscreen)
	{
		DEVMODE ss;
		memset(&ss, 0, sizeof(DEVMODE));
		ss.dmSize = sizeof(DEVMODE);
		ss.dmPelsWidth = w;
		ss.dmPelsHeight = h;
		ss.dmBitsPerPel = 32;
		ss.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		if (ChangeDisplaySettings(&ss, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) throw EXCEPTION("Could not create fullscreen window.");
		handle = CreateWindow(ClassName.c_str(), title.c_str(), WS_POPUP, 0, 0, w, h, NULL, NULL, instance, this);
		ShowCursor(false);
	}
	else
	{
		RECT rect = { 0, 0, w, h };
		AdjustWindowRect(&rect, style, true);
		handle = CreateWindow(ClassName.c_str(), title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, instance, this);
	}

	if (!handle) throw EXCEPTION("Could not create window.");
	ShowWindow(handle, SW_SHOWNORMAL);
	SetForegroundWindow(handle);
	SetFocus(handle);
	open = true;

	SetSize(width, height);
}

void Window::Close()
{
	DestroyWindow(handle);
	UnregisterClass(ClassName.c_str(), instance);
	open = false;
}

void Window::Update()
{
	MSG msg;
	while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::ClearEventHandlers()
{
	eventHandlers.Clear();
}

void Window::AddEventHandler(EventHandler eh)
{
	for (int i = 0; i < eventHandlers.Count(); i++)
	{
		if (eventHandlers[i] == eh) return;
	}
	eventHandlers.Add(eh);
}

void Window::RaiseEvent(int event, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < eventHandlers.Count(); i++) eventHandlers[i](*this, event, wParam, lParam);
}

void Window::SetTitle(string title)
{
	if (!open) return;
	SetWindowText(handle, title.c_str());
}

void Window::SetSize(int w, int h)
{
	if (!open) return;
	width = w;
	height = h;
	RECT rect = { 0, 0, w, h };
	AdjustWindowRect(&rect, style, menu != 0);
	MoveWindow(handle, x, y, rect.right - rect.left, rect.bottom - rect.top, true);
}

void Window::SetMenu(HMENU menu)
{
	if (!open) return;
	::SetMenu(handle, menu);
	if (this->menu) DestroyMenu(this->menu);
	this->menu = menu;
	SetSize(width, height);
}

bool Window::IsOpen() const
{
	return open;
}

HWND Window::GetHandle() const
{
	return handle;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

float Window::GetAspectRatio() const
{
	return (float) width / (float) height;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RaiseEvent(Msg, wParam, lParam);
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK Window::WndProcRouter(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Window *w = 0;

	switch (Msg)
	{
	case WM_GETMINMAXINFO:
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	case WM_NCCREATE:
		w = (Window *) ((LPCREATESTRUCT) lParam)->lpCreateParams;
		w->handle = hWnd;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) w);
		break;

	default:
		w = (Window *) GetWindowLong(hWnd, GWL_USERDATA);
	}
	if (!w) throw EXCEPTION("Window event router invoked without a window handle.");

	return w->WndProc(hWnd, Msg, wParam, lParam);
}

void Window::defaultEventHandler(Window& w, int Event, WPARAM wParam, LPARAM lParam)
{
	switch (Event)
	{
	case WM_MOVE:
		w.x = LOWORD(lParam);
		w.y = HIWORD(lParam);
		break;

	case WM_SIZE:
		w.width = LOWORD(lParam);
		w.height = HIWORD(lParam);
		break;

	case WM_CLOSE:
		PostQuitMessage(WM_QUIT);
		w.open = false;
	}
}
