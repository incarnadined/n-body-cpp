#include "pch.h"
#include "Skybox.h"

#include "Gdiplus.h"

bool Skybox::GdiInit()
{
	ULONG_PTR token = 0;
	Gdiplus::GdiplusStartupInput startupInput;
	if (Gdiplus::GdiplusStartup(&token, &startupInput, NULL) == Gdiplus::Status::Ok)
	{
		return true;
	}

	return false;
}

void Skybox::GdiShutdown()
{
	Gdiplus::GdiplusShutdown(0);
}

std::vector<std::vector<Colour>> Skybox::Load(std::wstring filepath)
{
	// filepath should point to a folder that contains 6 images for the faces, indexed 0.png-5.png
	// returns a vector of 6 vectors one for each file, an array of colours rgba format

	GdiInit();

	std::vector<std::vector<Colour>> images;
	images.reserve(6);

	for (size_t i = 0; i < 6; i++)
	{
		std::wstring file = filepath + L"\\" + std::to_wstring(i);
		Gdiplus::Bitmap image(file.c_str());

		if (image.GetLastStatus() != Gdiplus::Status::Ok)
		{
			throw;
		}


	}

	GdiShutdown();
}
