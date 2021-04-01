#include "pch.h"
#include "Clock.h"

Clock::Clock()
	:last(clock())
{
}

long Clock::Diff()
{
	auto delta = clock() - last;
	last += delta;

	return delta;
}
