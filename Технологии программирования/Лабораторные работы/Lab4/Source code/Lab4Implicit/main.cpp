#include <Windows.h>
#include <bits\stdc++.h>
#include "Widgets.h"
#include "Diff.h"

using std::make_shared;
using std::make_tuple;
using std::shared_ptr;
using std::tuple;
using std::tie;
using std::endl;

void WmCommandProc(WPARAM wParam, LPARAM lParam) {
	HWND hwnd = (HWND)lParam;
	switch (LOWORD(wParam)) {
	case BN_CLICKED:
		getWidget(hwnd)->onClick(wParam, lParam);
		break;
	default:
		break;
	}
}

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam) {

	switch (msg) {
	case WM_CLOSE:
		getWidget(hwnd)->hide();
		break;
	case WM_COMMAND:
		WmCommandProc(wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

tuple<
	shared_ptr<W32Window>,
	shared_ptr<W32TextEdit>>
	CreateTextViewWindow(
		const char* title,
		shared_ptr<W32Application> app,
		shared_ptr<W32Window> parent) {
	auto fileTextWindow = make_shared<W32Window>(title, 640, 520, *app, *parent);
	auto fileTextView = make_shared<W32TextEdit>(600, 440, 10, 10, *fileTextWindow);

	addWidget(fileTextWindow);
	addWidget(fileTextView);

	return make_tuple(fileTextWindow, fileTextView);
}

tuple<
	shared_ptr<W32Window>,
	shared_ptr<W32Button>,
	shared_ptr<W32TextField>,
	shared_ptr<W32Button>,
	shared_ptr<W32TextField>,
	shared_ptr<W32Button>>
	CreateMainWindow(shared_ptr<W32Application> app) {
	auto mainWindow = make_shared<W32Window>("Show differences", 620, 300, *app);
	auto openTextFile1 = make_shared<W32Button>("File...", 80, 30, 10, 10, *mainWindow);
	auto filePathEdit1 = make_shared<W32TextField>(480, 30, 100, 10, *mainWindow);
	auto openTextFile2 = make_shared<W32Button>("File...", 80, 30, 10, 90, *mainWindow);
	auto filePathEdit2 = make_shared<W32TextField>(480, 30, 100, 90, *mainWindow);
	auto openFilesButton = make_shared<W32Button>("Show", 80, 30, 10, 170, *mainWindow);

	addWidget(mainWindow);
	addWidget(openTextFile1);
	addWidget(filePathEdit1);
	addWidget(openTextFile2);
	addWidget(filePathEdit2);
	addWidget(openFilesButton);

	openTextFile1->setOnClickHandler([mainWindow, filePathEdit1](WPARAM w, LPARAM l) {
		auto path = getOpenFileName(*mainWindow, "All\0 * .*\0Text\0 * .txt\0");
		filePathEdit1->setText(path);
	});

	openTextFile2->setOnClickHandler([mainWindow, filePathEdit2](WPARAM w, LPARAM l) {
		auto path = getOpenFileName(*mainWindow, "All\0 * .*\0Text\0 * .txt\0");
		filePathEdit2->setText(path);
	});

	return make_tuple(mainWindow, openTextFile1, filePathEdit1, openTextFile2, filePathEdit2, openFilesButton);
}

shared_ptr<W32Application> CreateApplication(HINSTANCE hInstance, int nCmdShow) {
	auto app = make_shared<W32Application>("Lab3", WndProc, hInstance);

	// main window
	shared_ptr<W32Window> mainWindow;
	shared_ptr<W32Button> openTextFile1;
	shared_ptr<W32TextField> filePathEdit1;
	shared_ptr<W32Button> openTextFile2;
	shared_ptr<W32TextField> filePathEdit2;
	shared_ptr<W32Button> openFilesButton;
	tie(
		mainWindow,
		openTextFile1,
		filePathEdit1,
		openTextFile2,
		filePathEdit2,
		openFilesButton) = CreateMainWindow(app);

	// first text view window: tuple <window, textedit>
	auto fileTextWindow1 = CreateTextViewWindow("Text 1", app, mainWindow);

	// second text view window: tuple <window, textedit>
	auto fileTextWindow2 = CreateTextViewWindow("Text 2", app, mainWindow);

	openFilesButton->setOnClickHandler([=](WPARAM w, LPARAM l) {
		std::hash<std::string> hash;
		auto text1 = LoadTextFromFile(filePathEdit1->text());
		auto text2 = LoadTextFromFile(filePathEdit2->text());

		for (auto& c : text1) {
			c = ansi_toupper(c);
		}
		for (auto& c : text2) {
			c = ansi_toupper(c);
		}

		if (hash(text1) == hash(text2)) {
			MessageBox(
				mainWindow->hwnd(),
				"Тексты совпадают",
				"Информация",
				MB_ICONINFORMATION);
			return;
		}

		auto diffs = ShowDifferences(text1, text2);

		std::get<1>(fileTextWindow1)->setText(diffs.first);
		std::get<1>(fileTextWindow2)->setText(diffs.second);
		std::get<1>(fileTextWindow1)->setReadOnly(TRUE);
		std::get<1>(fileTextWindow2)->setReadOnly(TRUE);

		std::get<0>(fileTextWindow1)->setModal(TRUE);
		std::get<0>(fileTextWindow2)->setModal(TRUE);
		std::get<0>(fileTextWindow1)->show();
		std::get<0>(fileTextWindow2)->show();
	});

	mainWindow->show(nCmdShow);

	return app;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	auto app = CreateApplication(hInstance, nCmdShow);

	return app->exec();
}