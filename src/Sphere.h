#pragma once
#include "pch.h"

#include "Vec3.h"

class Sphere
{
public:
	Sphere(float radius, Vec3f position);
	~Sphere();

	void Move(Vec3f translation);
	void Void(); // void the circle (delete it)

	// Getters/Setters
	void SetColour(Colour colour);
	std::tuple<float, Colour, Vec3f> GetData();

private:
	float mRadius;
	Vec3f mPosition;
	Colour mColour;

	bool mVoid;
};

