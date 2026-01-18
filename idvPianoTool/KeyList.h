#pragma once
#include <vector>
#include <algorithm>
#include <windows.h>
#include <mmsystem.h>
#include "KeyInput.h"
#include "Note.h"
#include "Level2Vk.h"
#include "RandomDelay.h"

#pragma comment(lib, "winmm.lib")

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
			return -1;
		}
		Sleep(100);
		SetForegroundWindow(hd);
		std::cout << "opened\n";
		Sleep(1000);
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
			deltaT = i.delay - 1000.0 * (end.QuadPart - TimeStart.QuadPart) / Fre.QuadPart + RandomDelay::randomDelay();
			if (deltaT > 1) {
				Sleep(deltaT);
			}
			if (i.dwFlag == 0) {
				A.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &A, sizeof(INPUT));
				A.ki.dwFlags = 0;
			}
			SendInput(1, &A, sizeof(INPUT));
		}
		timeEndPeriod(1);
		return 0;
	}
};