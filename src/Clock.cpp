#include "pch.h"
#include "Clock.h"

Clock::Clock()
	:mOldTime(std::chrono::high_resolution_clock::now())
{
}

double Clock::Diff()
{
	auto diff = std::chrono::high_resolution_clock::now() - mOldTime;
	mOldTime += diff;

	return std::chrono::duration<double, std::milli>(diff).count();
}
