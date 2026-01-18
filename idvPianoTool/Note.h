#pragma once
#include <iostream>
#include <string>
#include <vector>

class Note {
public:
	char level;
	int scale;
	double start;
	double end;
	//static std::set<int> scales;
	static int scales[11];
	Note(std::string l, double s, double e) {
		level = l[0];
		if (l[1] == '#') {
			level -= ('A' - 'a');
			scale = l[2] - '0';
		}
		else {
			scale = l[1] - '0';
		}
		start = s;
		end = e;
		//scales.insert(scale);
		scales[scale]++;
	}
	void printNote(void) {
		std::cout << level << ' ' << scale << ' ' << start << ' ' << end << '\n';
	}
};