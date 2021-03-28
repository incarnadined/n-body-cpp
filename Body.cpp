#include "pch.h"
#include "Body.h"

Body::Body(double mass, float radius, Vec3f position, Colour colour)
	: mMass(mass), mRadius(radius), mPosition(position), mColour(colour)
{
}

Body::~Body()
{
}

void Body::Move(Vec3f translation)
{
	mPosition += translation;
}

std::tuple<float, Colour, Vec3f> Body::GetData()
{
	return std::make_tuple(mRadius, mColour, mPosition);
}

double Body::GetMass()
{
	return mMass;
}

float Body::GetRadius()
{
	return mRadius;
}

Vec3f Body::GetPosition()
{
	return mPosition;
}

void Body::CalculateForce(Body& other, float dt)
{
	double force_mag = (G * mMass * other.GetMass()) / std::pow((mPosition - other.GetPosition()).magnitude(), 2);
	Vec3f direction = (mPosition - other.GetPosition()).normalise();
	
	ApplyForce(direction * force_mag, dt);
	other.ApplyForce(-direction * force_mag, dt);
}

void Body::ApplyForce(Vec3f force, float dt)
{
	mVelocity += ((force / mMass) * dt);
	mPosition += mVelocity * dt;
}