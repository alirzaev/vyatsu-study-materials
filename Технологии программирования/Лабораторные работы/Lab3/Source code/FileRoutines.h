#pragma once

#include <Windows.h>
#include <bits\stdc++.h>

extern std::ofstream logs;

using tstring = std::basic_string < TCHAR > ;

void UStrCpy(TCHAR* dst, const char* src, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		*dst = *src;
		++dst;
		++src;
	}
}

std::string LoadTextFromFile(const std::string& name) {
	std::ifstream file(name);
	std::string text;
	std::string line;
	while (std::getline(file, line)) {
		text += line;
		text += "\r\n";
	}
	
	return text;
}