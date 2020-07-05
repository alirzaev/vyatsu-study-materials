#pragma once
#include <bits\stdc++.h>
#include <Windows.h>

using std::make_shared;
using std::shared_ptr;
using std::endl;

class W32Widget;
shared_ptr<W32Widget> getWidget(HWND hwnd);

class WException : public std::exception {
public:
	WException() = default;
	WException(const char* msg) : std::exception(msg) {}
};

class W32Application {
private:
	WNDCLASSEX mWndCl;
public:
	W32Application() = delete;
	W32Application(const char* name, WNDPROC wndProc, HINSTANCE hInstance) {
		mWndCl.cbSize = sizeof(WNDCLASSEX);
		mWndCl.style = 0;
		mWndCl.lpfnWndProc = wndProc;
		mWndCl.cbClsExtra = 0;
		mWndCl.cbWndExtra = 0;
		mWndCl.hInstance = hInstance;
		mWndCl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		mWndCl.hCursor = LoadCursor(NULL, IDC_ARROW);
		mWndCl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		mWndCl.lpszMenuName = NULL;
		mWndCl.lpszClassName = name;
		mWndCl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&mWndCl)) {
			throw WException();
		}
	}

	HINSTANCE instance() const {
		return mWndCl.hInstance;
	}

	const char* name() const {
		return mWndCl.lpszClassName;
	}

	WPARAM exec() {
		MSG Msg;
		while (GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return Msg.wParam;
	}
};

class W32Widget {
public:
	using Handler = std::function < void(WPARAM, LPARAM) >;

protected:
	HWND mHwnd;
	Handler mOnClickHandler;

public:
	HWND hwnd() const {
		return mHwnd;
	}

	virtual bool show(int nCmdShow = SW_SHOW) {
		return ShowWindow(mHwnd, nCmdShow);
	}

	virtual bool update() {
		return UpdateWindow(mHwnd);
	}

	virtual void destroy() {
		DestroyWindow(mHwnd);
	}

	virtual void hide() {
		ShowWindow(mHwnd, SW_HIDE);
	}

	void setOnClickHandler(Handler handler) {
		mOnClickHandler = handler;
	}

	void onClick(WPARAM wParam, LPARAM lParam) {
		if (mOnClickHandler) {
			mOnClickHandler(wParam, lParam);
		}
	}
};

class W32Window : public W32Widget {
private:
	bool mModal = false;
	int modalChildren = 0;
	W32Window(const char* title, int width, int height, const W32Application& app, HWND parent) {
		mHwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			app.name(), title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			parent, NULL, app.instance(), NULL);

		if (mHwnd == NULL) {
			throw WException();
		}
	}

	void incModalChildren() {
		++modalChildren;
		EnableWindow(mHwnd, FALSE);
	}

	void decModalChildren() {
		if (modalChildren > 0) {
			--modalChildren;
		}
		if (modalChildren == 0) {
			EnableWindow(mHwnd, TRUE);
		}
	}

public:
	W32Window() = default;
	W32Window(const char* title, int width, int height, const W32Application& app)
		: W32Window(title, width, height, app, NULL) {}
	W32Window(const char* title, int width, int height, const W32Application& app, const W32Widget& parent)
		: W32Window(title, width, height, app, parent.hwnd()) {}

	void setModal(bool flag) {
		W32Window& parent = dynamic_cast<W32Window&>(*getWidget(GetWindow(mHwnd, GW_OWNER)));
		mModal = flag;
		if (flag) { // disable parent window
			parent.incModalChildren();
		}
		else { // enable parent window
			parent.decModalChildren();
		}
	}

	bool modal() const {
		return mModal;
	}

	void hide() override {
		if (GetWindow(mHwnd, GW_OWNER) == NULL) {
			destroy();
		}
		else {
			W32Widget::hide();
			if (mModal) {
				setModal(FALSE);
			}
		}
	}

	//bool show(int show = SW_SHOW) override {
	//	bool res = W32Widget::show(show);
	//	BringWindowToTop(mHwnd);

	//	return res;
	//}
};

class W32Button : public W32Widget {
public:
	W32Button() = default;
	W32Button(
		const char* label,
		int width, int height,
		int x, int y,
		const W32Widget parent) {
		mHwnd = CreateWindow(
			"BUTTON", label,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			x, y, width, height,
			parent.hwnd(), NULL,
			(HINSTANCE)GetWindowLong(parent.hwnd(), GWL_HINSTANCE), NULL);

		if (mHwnd == NULL) {
			throw WException();
		}
	}
};

class W32AbstractTextEdit : public W32Widget {
public:
	bool readOnly() const {
		return ES_READONLY & GetWindowLong(mHwnd, GWL_STYLE);
	}

	void setReadOnly(bool flag) {
		SendMessage(mHwnd, EM_SETREADONLY, (flag ? TRUE : FALSE), 0);
	}

	void setText(const std::string& text) {
		SetWindowTextA(mHwnd, text.c_str());
	}

	void setText(const char* text) {
		SetWindowTextA(mHwnd, text);
	}

	std::string text() const {
		int length = GetWindowTextLengthA(mHwnd);
		std::vector<char> buffer(length + 1);
		GetWindowTextA(mHwnd, buffer.data(), length + 1);
		return std::string(buffer.begin(), buffer.end() - 1);
	}
};

class W32TextEdit : public W32AbstractTextEdit {
public:
	W32TextEdit() = default;
	W32TextEdit(
		int width, int height,
		int x, int y,
		const W32Widget parent) {
		mHwnd = CreateWindow(
			"EDIT", "",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL |
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
			x, y, width, height,
			parent.hwnd(), NULL,
			(HINSTANCE)GetWindowLong(parent.hwnd(), GWL_HINSTANCE), NULL);

		if (mHwnd == NULL) {
			throw WException();
		}
	}
};

class W32TextField : public W32AbstractTextEdit {
public:
	W32TextField() = default;
	W32TextField(
		int width, int height,
		int x, int y,
		const W32Widget parent) {
		mHwnd = CreateWindow(
			"EDIT", "",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER |
			ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			x, y, width, height,
			parent.hwnd(), NULL,
			(HINSTANCE)GetWindowLong(parent.hwnd(), GWL_HINSTANCE), NULL);

		if (mHwnd == NULL) {
			throw WException();
		}
	}
};

std::map<HWND, shared_ptr<W32Widget>> widgets;

void addWidget(shared_ptr<W32Widget> widget) {
	widgets.emplace(widget->hwnd(), widget);
}

shared_ptr<W32Widget> getWidget(HWND hwnd) {
	return widgets.at(hwnd);
}

std::string getOpenFileName(const W32Widget& parent, const char* filters) {
	OPENFILENAME ofn;
	const size_t pathSz = 260;
	char path[pathSz];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent.hwnd();
	ofn.lpstrFile = path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = pathSz;
	ofn.lpstrFilter = filters;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
		return std::string(path, path + pathSz);
	}
	else {
		return "";
	}
}