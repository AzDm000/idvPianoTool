#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
int print2hd(HWND hd, std::wstring in);

std::wstring s2ws(const std::string& str);

class KeyInput {
public:
	DWORD dwFlag;
	WORD wVk;
};

class KeyList {
public:
	std::vector<KeyInput> KeyL;
	static INPUT EMPTY_INPUT;
	int pushInput(const KeyInput& a) {
		KeyL.push_back(a);
		return KeyL.size();
	}
	int pushInput(WORD a, DWORD b) {
		KeyInput tmp;
		tmp.dwFlag = b;
		tmp.wVk = a;
		KeyL.push_back(tmp);
		return KeyL.size();
	}
	int print2hd(HWND hd) {
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
		int cnt = 0;
		for (auto i : KeyL) {
			A.ki.wVk = i.wVk;
			A.ki.dwFlags = i.dwFlag;
			SendInput(1, &A, sizeof(INPUT));
			cnt++;
			Sleep(70);
		}
		return cnt;
	}
};