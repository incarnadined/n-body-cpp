#pragma once
#include "pch.h"

class Clock
{
public:
	Clock();
	long Diff();

private:
	clock_t last;
};

