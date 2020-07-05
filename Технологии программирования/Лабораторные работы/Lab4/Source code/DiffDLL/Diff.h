#pragma once

#include <Windows.h>
#include <bits\stdc++.h>

__declspec(dllimport) std::string LoadTextFromFile(const std::string& name);

__declspec(dllimport) int ansi_tolower(int ch);

__declspec(dllimport) int ansi_toupper(int ch);

__declspec(dllimport) std::pair<
	std::string,
	std::string
> ShowDifferences(const std::string& text1, const std::string& text2);