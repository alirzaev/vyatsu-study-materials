#pragma once
#include <bits\stdc++.h>
#include <Windows.h>
#include "Widgets.h"

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
