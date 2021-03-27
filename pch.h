#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>

#include <windows.h>
#include <d3d11.h>
#include <wrl.h>


/// Custom structs
struct Colour 
{
	Colour(float r, float g, float b)
		: r(r), g(g), b(b), a(1.0f) {}
	Colour(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a) {}
	float r;
	float g;
	float b;
	float a;
};