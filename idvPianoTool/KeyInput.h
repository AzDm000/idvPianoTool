#pragma once
#include <windows.h>
#include <iostream>

class KeyInput {
public:
	DWORD dwFlag;
	WORD wVk;
	double delay; //ms
	static bool compare(const KeyInput& a, const KeyInput& b) {
		return a.delay < b.delay;
	}
	void printKeyInput(void) {
		std::cout << dwFlag << ' ' << wVk << ' ' << delay << '\n';
	}
};
