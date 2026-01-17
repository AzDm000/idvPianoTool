#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include <locale>
#include <codecvt>
#include <set>
#include <fstream>
#include <cstdlib>
#include "json.hpp"
int print2hd(HWND hd, std::wstring in);

std::wstring s2ws(const std::string& str);

#pragma pack(push, 1)
class KeyInput {
public:
	DWORD dwFlag;
	WORD wVk;
	double delay; //ms
	static bool compare(const KeyInput& a, const KeyInput& b) {
		return a.delay < b.delay;
	}
	void printKeyInput(void) {
		std::cout << dwFlag << ' ' <<  wVk << ' ' << delay << '\n';
	}
};
#pragma pack(pop)

class Level2Vk {
public:
	static int mode;
	static std::map<std::pair<char, int>, WORD> mp1;
	static std::map<std::pair<char, int>, WORD> mp2;
	static WORD toVk(const char& level, const int& scale) {
		if (mode == 1) {
			return mp1[{level, scale}];
		}
		else if (mode == 2) {
			return mp2[{level, scale}];
		}
		else {
			return 0;
		}
	}
};

class Note {
public:
	char level;
	int scale;
	double start;
	double end;
	static std::set<int> scales;
	Note(std::string l, double s, double e) {
		level = l[0];
		scale = l[1];
		start = s;
		end = e;
		scales.insert(scale);
	}
	void printNote(void) {
		std::cout << level << ' ' << scale << ' ' << start << ' ' << end << '\n';
	}
};

class KeyList {
public:
	std::vector<KeyInput> KeyL;
	static INPUT EMPTY_INPUT;
	int pushInput(const KeyInput& a) {
		KeyL.push_back(a);
		return KeyL.size();
	}
	int pushInput(WORD a, DWORD b, double c) {
		KeyInput tmp;
		tmp.dwFlag = b;
		tmp.wVk = a;
		tmp.delay = c;
		KeyL.push_back(tmp);
		//std::cout << "pushed :" << 
		return KeyL.size();
	}
	int pushInput(Note& note) {
		this->pushInput(Level2Vk::toVk(note.level, note.scale), 0, note.start);
		return this->pushInput(Level2Vk::toVk(note.level, note.scale), KEYEVENTF_KEYUP, note.end);
	}
	void sortself(void) {
		std::sort(KeyL.begin(), KeyL.end(), KeyInput::compare);
	}
	void printKeyList(void) {
		for (auto& i : KeyL) {
			i.printKeyInput();
		}
	}
	int print2hd(HWND hd) {
		if (!IsWindow(hd)) {
			return 0;
		}
		Sleep(100);
		SetForegroundWindow(hd);
		std::cout << "opened\n";
		Sleep(100);
		LARGE_INTEGER Fre, end;
		QueryPerformanceFrequency(&Fre);
		INPUT A;
		ZeroMemory(&A, sizeof(INPUT));
		A.type = INPUT_KEYBOARD;
		LARGE_INTEGER TimeStart;
		double deltaT = 0;
		timeBeginPeriod(1);
		QueryPerformanceCounter(&TimeStart);
		for (auto& i : KeyL) {
			A.ki.dwFlags = i.dwFlag;
			A.ki.wVk = i.wVk;
			QueryPerformanceCounter(&end);
			deltaT = i.delay - 1.0 * (end.QuadPart - TimeStart.QuadPart) / Fre.QuadPart;
			if (deltaT > 1) {
				Sleep(deltaT);
			}
			SendInput(1, &A, sizeof(INPUT));
		}
		timeEndPeriod(1);
	}
};

