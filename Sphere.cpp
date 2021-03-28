#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(float radius, Vec3f position)
	: mRadius(radius), mPosition(position), mColour(1.0f, 0, 0), mVoid(false)
{
}

Sphere::~Sphere()
{
}

void Sphere::Move(Vec3f translation)
{
	mPosition += translation;
}

void Sphere::Void()
{
	mVoid = true;
}

void Sphere::SetColour(Colour colour)
{
	mColour = colour;
}

std::tuple<float, Colour, Vec3f> Sphere::GetData()
{
	if (mVoid)
		return std::make_tuple(0, Colour(0.0f, 0.0f, 0.0f), Vec3f(0, 0, 0));
	return std::make_tuple(mRadius, mColour, mPosition);
}
