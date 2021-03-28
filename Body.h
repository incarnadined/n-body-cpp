#pragma once
#include "pch.h"

#include "Vec3.h"

class Body
{
public:
	Body(double mMass, float mRadius, Vec3f position, Colour colour);
	~Body();

	void Move(Vec3f translation);
	std::tuple<float, Colour, Vec3f> GetData();

	double GetMass();
	float GetRadius();
	Vec3f GetPosition();

	void CalculateForce(Body& other, float dt);
	void ApplyForce(Vec3f force, float dt);

	// i dont know why i need this
	bool operator<(Body other) { return mMass < other.GetMass(); }

private:
	double mMass;
	float mRadius;
	Vec3f mPosition;
	Vec3f mVelocity;
	Colour mColour;
};

