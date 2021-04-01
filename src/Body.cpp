#include "pch.h"
#include "Body.h"

Body::Body(double mass, float radius, Vec3f position, Colour colour)
	: mMass(mass), mRadius(radius), mPosition(position), mColour(colour), mInitialPos(position)
{
}

Body::~Body()
{
}

void Body::Reset()
{
	mPosition = mInitialPos;
	mVelocity = Vec3f();
}

void Body::Move(Vec3f translation)
{
	mPosition += translation;
}

std::tuple<float, Vec3f, Colour> Body::GetData()
{
	return std::make_tuple(mRadius, mPosition, mColour);
}

void Body::DrawImGui(int index)
{
	ImGui::Text("Body %i", index);
	ImGui::BulletText("Pos X: %f", mPosition.GetX());
	ImGui::BulletText("Pos Y: %f", mPosition.GetY());
	ImGui::BulletText("Pos Z: %f", mPosition.GetZ());
	ImGui::ColorEdit3("Colour", mColour.farray());
}

double Body::GetMass()
{
	return mMass;
}

float Body::GetRadius()
{
	return mRadius;
}

Colour Body::GetColour()
{
	return mColour;
}

void Body::SetColour(Colour colour)
{
	mColour = colour;
}

Vec3f Body::GetPosition()
{
	return mPosition;
}

void Body::ApplyForce(Body& other, float dt)
{
	double force_mag = (G * mMass * other.GetMass()) / std::pow((mPosition - other.GetPosition()).magnitude(), 2);
	Vec3f direction = (mPosition - other.GetPosition()).normalise();
	
	ApplyForce(-direction * force_mag, dt);
	other.ApplyForce(direction * force_mag, dt);
}

void Body::ApplyForce(Vec3f force, float dt)
{
	mVelocity += ((force / mMass) * dt);
	mPosition += mVelocity * dt;
}