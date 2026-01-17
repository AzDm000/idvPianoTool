#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include "files.h"
//main1
int main1() {
	std::string a;
	wchar_t hdName[] = L"新建文本文档.txt - 记事本";
	std::cout << "input a char string here:\n";
	std::cin >> a;
	KeyList L;
	for (auto i : a) {
		if (i >= 'a' && i <= 'z') {
			L.pushInput((i + 'A' - 'a'), 0);
			L.pushInput(i + 'A' - 'a', KEYEVENTF_KEYUP);
		}
		else if ((i >= '0' && i <= '9')) {
			L.pushInput(i, 0);
			L.pushInput(i, KEYEVENTF_KEYUP);
		}
	}
	int maxT = 100;
	HWND hd;
	for (int i = 0; i < maxT; i++) {
		hd = FindWindowW(nullptr, hdName);
		Sleep(10);
		if (IsWindow(hd)) {
			L.print2hd(hd);
			std::cout << "End main";
			break;
		}
	}
	return 0;
}
//main2
int main2() {
	INPUT a;
	HWND hd = FindWindowW(nullptr, L"新建文本文档.txt - 记事本");
	ZeroMemory(&a, sizeof(a));
	a.type = INPUT_MOUSE;
	a.mi.dx = 0;
	a.mi.dy = 0;
	a.mi.dwFlags = MOUSEEVENTF_MOVE;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 65535; j++) {
			SendInput(1, &a, sizeof(INPUT));
			a.mi.dx--;
			a.mi.dy--;
			Sleep(10);
		}
		a.mi.dx = 0; a.mi.dy = 0;
	}
	return 0;
}
//main3
int main() {
	SetProcessDPIAware();
	INPUT a;
	ZeroMemory(&a, sizeof(INPUT));
	a.type = INPUT_MOUSE;
	int WindowWidth = GetSystemMetrics(SM_CXSCREEN);
	int WindowLength = GetSystemMetrics(SM_CYSCREEN);
	std::cout << WindowWidth << " " << WindowLength;
}
/*
int main() {
	int flag = 0;
	int cnt = 0;

	int inputLen = 12;
	INPUT* inputs = new INPUT[inputLen];
	for (int i = 0; i < inputLen; i++) {
		ZeroMemory(&inputs[i], sizeof(INPUT));
		inputs[i].type = INPUT_KEYBOARD;
	}
	//set inputs here
	inputs[0].ki.dwFlags = 0;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[0].ki.wVk = VK_CONTROL;
	inputs[1].ki.wVk = VK_CONTROL;
	inputs[2].ki.dwFlags = 0;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[2].ki.wVk = 'A';
	inputs[3].ki.wVk = 'A';
	inputs[4].ki.dwFlags = 0;
	inputs[5].ki.dwFlags = 0;
	inputs[4].ki.wVk = VK_CONTROL;
	inputs[5].ki.wVk = 'S';
	inputs[6].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[7].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[6].ki.wVk = 'S';
	inputs[7].ki.wVk = VK_CONTROL;
	inputs[8].ki.dwFlags = 0;
	inputs[9].ki.dwFlags = 0;
	inputs[8].ki.wVk = VK_MENU;
	inputs[9].ki.wVk = VK_F4;
	inputs[10].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[11].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[10].ki.wVk = VK_MENU;
	inputs[11].ki.wVk = VK_F4;
	int inputFlag = 0;
	//char hdName[] = "新建文本文档.txt - 记事本";
	HWND hd;
	std::cout << "input anything to start.";
	Sleep(200);
	int a;
	std::cin >> a;
	while(cnt < 1000) {
		hd = FindWindowW(nullptr, L"新建文本文档.txt - 记事本");
		if (IsWindow(hd)) {
			SetForegroundWindow(hd);
			Sleep(1000);
			for(int i = 0; i < inputLen; i++) {
				SendInput(1, &inputs[i], sizeof(INPUT));
				Sleep(100);
			}

			flag = 1;
			break;
		}
		else {
			cnt++;
			Sleep(10);
		}
	}

	if (flag == 1) {
		std::cout << "target window finded!\nCost " << cnt << " time(s) loop.";
	}
	else {
		std::cout << "target window not found!\nCost " << cnt << " times loop.";
	}
}*/