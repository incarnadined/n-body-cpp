#pragma once
#include "pch.h"

class Skybox
{
public:
	static ImageData Load(std::wstring filepath);

private:
	static bool GdiInit();
	static void GdiShutdown();
};

