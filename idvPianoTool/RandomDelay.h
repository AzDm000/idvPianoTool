#pragma once
#include <random>
#include <time.h>

class RandomDelay {
public:
	static std::mt19937 gen;
	static std::normal_distribution<double> dis;
	static double randomDelay(void) {
		double a = dis(gen);
		if (abs(a) > 5)a = 0;
		return a;
	}
};