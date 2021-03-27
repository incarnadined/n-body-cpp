#pragma once

#define NOMINMAX

#include <iostream>
#include <vector>
#include <functional>
#include <string>

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>


/// Custom structs
struct Colour 
{
	Colour(float r, float g, float b)
		: r(r), g(g), b(b), a(1.0f) {}
	Colour(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a) {}
	Colour(unsigned char r, unsigned char g, unsigned char b)
		: r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(1.0f) {}
	Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}
	float r;
	float g;
	float b;
	float a;
};

// define _uc suffix for unsigned chars
inline constexpr unsigned char operator "" _uc(unsigned long long arg) noexcept
{
	return static_cast<unsigned char>(arg);
}