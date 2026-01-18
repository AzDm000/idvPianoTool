#pragma once
#include <windows.h>
#include <map>

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