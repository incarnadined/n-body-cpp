#include "pch.h"
#include "Skybox.h"

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

ImageData Skybox::Load(std::wstring filepath)
{
	// filepath should point to a folder that contains 6 images for the faces, indexed 0.png-5.png
	// returns a vector of 6 vectors one for each file, an array of colours rgba format
	ImageData returnData = {};

	GdiInit();

	std::vector<std::vector<Colour>> images = 
	{
		{}, {}, {}, {}, {}, {}
	};

	for (size_t i = 0; i < 6; i++)
	{
		std::wstring file = filepath + L"\\" + std::to_wstring(i) + L".png";
		Gdiplus::Bitmap image(file.c_str());

		if (image.GetLastStatus() != Gdiplus::Status::Ok)
		{
			throw;
		}

		size_t width = image.GetWidth();
		size_t height = image.GetHeight();
		returnData.width = width;
		returnData.height = height;
		images[i].reserve(width * height);

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				image.GetPixel(x, y, &c);
				images[i].push_back({ c.GetValue() });
			}
		}
	}

	GdiShutdown();

	returnData.data = images;

	return returnData;
}
