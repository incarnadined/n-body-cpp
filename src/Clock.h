#pragma once
#include "pch.h"

class Clock
{
public:
	Clock();
	double Diff();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mOldTime;
};

