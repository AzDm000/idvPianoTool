#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include "files.h"
#include "json.hpp"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//main1
using json = nlohmann::json;
std::string path = "C:\\Users\\Moonlight\\Desktop";
/*int main1() {
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
}*/
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
int main3() {
	SetProcessDPIAware();
	INPUT a;
	HWND hd;
	ZeroMemory(&a, sizeof(INPUT));
	a.type = INPUT_MOUSE;
	int WindowWidth = GetSystemMetrics(SM_CXSCREEN);
	int WindowHeight = GetSystemMetrics(SM_CYSCREEN);
	int inX, inY;
	int outX, outY;
	while (1) {
		std::cout << "Enter 2 nunbers (-1 to break, -2 to click):\n";
		std::cin >> inX;
		if (inX == -1)break;
		if (inX == -2) {
			a.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
			SendInput(1, &a, sizeof(INPUT));
			continue;
		}
		std::cin >> inY;
		outX = inX * 65535 / (WindowWidth - 1);
		outY = inY * 65535 / (WindowHeight - 1);
		std::cout << outX << ' ' << outY << '\n';
		//hd = FindWindowW(nullptr, L"新建文本文档.txt - 记事本");
		a.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		a.mi.dx = outX;
		a.mi.dy = outY;
		SendInput(1, &a, sizeof(INPUT));
	}
	return 0;
}
//main4
int main4() {
	RegisterHotKey(nullptr, 999, MOD_ALT | MOD_NOREPEAT, 'S');
	RegisterHotKey(nullptr, 1000, MOD_ALT | MOD_NOREPEAT, 'Q');
	MSG msg;
	while (1) {
		std::cout << "loop in\n";
		GetMessage(&msg, nullptr, 0, 0);
		std::cout << "here\n";
		if (msg.message == WM_HOTKEY) {
			if (msg.wParam == 999) {
				std::cout << "Status : Active\n";
			}
			else if (msg.wParam == 1000) {
				std::cout << "Leaving\n";
				Sleep(700);
				UnregisterHotKey(nullptr, 999);
				UnregisterHotKey(nullptr, 1000);
				return 0;
			}
		}
	}
	return 0;
}
//main5
int main5() {
	RegisterHotKey(nullptr, 100, MOD_ALT, 'Q');
	RegisterHotKey(nullptr, 101, MOD_ALT, 'S');
	LARGE_INTEGER tot;
	QueryPerformanceFrequency(&tot);
	MSG msg;
	timeBeginPeriod(1);

	while (1) {
		GetMessage(&msg, nullptr, 0, 0);
		if (msg.wParam == 100) {
			std::cout << "Leaving\n";
			Sleep(700);
			return 0;
		}
		else {
			std::cout << "input a sleeping number(ms) :\n";
			int a;
			std::cin >> a;
			LARGE_INTEGER begin, end;
			QueryPerformanceCounter(&begin);
			Sleep(a);
			QueryPerformanceCounter(&end);
			std::cout << "begin: " << begin.QuadPart << ", end: " << end.QuadPart << '\n';
			std::cout << "total seconds:" << 1.0 * (end.QuadPart - begin.QuadPart) / tot.QuadPart;
		}
	}
	timeEndPeriod(1);
	UnregisterHotKey(nullptr, 100);
	UnregisterHotKey(nullptr, 101);
	return 0;
}
//main6
int reachfile(const std::string& command, const std::string& targetpath, json& j) {
	int flag = std::system(command.c_str());
	if (flag == 1) {
		std::cerr << "flp转换失败.\n";
		return -1;
	}
	std::ifstream ijson(targetpath);
	if (!ijson.is_open()) {
		std::cerr << "Can't open file: " << targetpath << '\n';
		return -1;
	}
	try {
		ijson >> j;
	}
	catch (json::parse_error& e) {
		std::cerr << "Parse_error: " << e.what() << '\n';
		return -1;
	}
	ijson.close();
	return 0;
}

std::set<int> Note::scales;
std::vector<Note> NoteList;
int Level2Vk::mode = 1;
std::map<std::pair<char, int>, WORD> Level2Vk::mp1;
std::map<std::pair<char, int>, WORD> Level2Vk::mp2;
int main() {
	
	std::string exporterpath = "flp_exporter.exe";
	std::string outpath = "out.json";

	std::cout << "input your target file:\n";
	std::string get;
	std::cin >> get;
	json j;
	if (reachfile((exporterpath + ' ' + get + ' ' + outpath).c_str(), outpath, j) != 0) {
		std::cerr << "程序已停止运行\n";
		return -1;
	}

	KeyList keys;

	//print all json contents;
	for (int i = 0; i < j.size(); i++) {
		std::cout << j[i]["pitch"] << ' ' << j[i]["start_ms"] << ' ' << j[i]["end_ms"] << '\n';
	}

	for (int i = 0; i < j.size(); i++) {
		Note tmp(j[i]["pitch"], j[i]["start_ms"], j[i]["end_ms"]);
		keys.pushInput(tmp);
	}
	std::cout << "--------\n";
	keys.printKeyList();
	std::cout << "-------\n";
	keys.sortself();
	keys.printKeyList();
	return 0;
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