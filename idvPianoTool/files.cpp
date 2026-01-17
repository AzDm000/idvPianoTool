#pragma once
#include<windows.h>
#include<string>
#include "files.h"

int print2hd(HWND hd, std::wstring in) {
	if (!IsWindow(hd)) {
		return 0;
	}
	Sleep(100);
	SetForegroundWindow(hd);
	std::cout << "opened\n";
	Sleep(100);
	INPUT A;
	ZeroMemory(&A, sizeof(INPUT));
	A.type = INPUT_KEYBOARD;
	A.ki.dwFlags = KEYEVENTF_UNICODE;
	int cnt = 0;
	for (auto i : in) {
		A.ki.wScan = i;
		A.ki.dwFlags = KEYEVENTF_UNICODE;
		SendInput(1, &A, sizeof(INPUT));
		A.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
		SendInput(1, &A, sizeof(INPUT));
		cnt++;
		Sleep(70);
	}
	return cnt;
}

std::wstring s2ws(const std::string& str) {
	if (str.empty()) return L"";
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
