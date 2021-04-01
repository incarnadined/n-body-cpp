#pragma once
#include "pch.h"

#include "Vec3.h"

class Body
{
public:
	Body(double mMass, float mRadius, Vec3f position, Colour colour);
	~Body();

	void Move(Vec3f translation);
	std::tuple<float, Vec3f, Colour> GetData();

	double GetMass();
	float GetRadius();
	Colour GetColour();
	void SetColour(Colour colour);
	Vec3f GetPosition();

	void ApplyForce(Body& other, float dt); // applies a force and calls the second overload for the other body with the precalculated force
	void ApplyForce(Vec3f force, float dt);

	bool operator<(Body other) { return mMass < other.GetMass(); }

private:
	double mMass;
	float mRadius;
	Colour mColour;
	Vec3f mPosition;
	Vec3f mVelocity;
};

