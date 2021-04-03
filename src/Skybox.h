#pragma once
#include "pch.h"

class Skybox
{
public:
	static std::vector<std::vector<Colour>> Load(std::wstring filepath);

private:
	static bool GdiInit();
	static void GdiShutdown();
};

