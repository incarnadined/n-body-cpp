#include "pch.h"
#include "Vec3.h"

Vec3f::Vec3f()
	: x(0), y(0), z(0)
{
}

Vec3f::Vec3f(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vec3f::Vec3f(double x, double y, double z)
	: x((float)x), y((float)y), z((float)z)
{
}

Vec3f::~Vec3f()
{
}

float Vec3f::GetX()
{
	return x;
}

float Vec3f::GetY()
{
	return y;
}

float Vec3f::GetZ()
{
	return z;
}

float Vec3f::magnitude()
{
	return std::pow(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f), 0.5f);
}

Vec3f Vec3f::normalise()
{
	return Vec3f(x / magnitude(), y / magnitude(), z / magnitude());
}

DirectX::XMVECTOR Vec3f::DX(float w)
{
	// returns an XMVECTOR of the current vector
	return DirectX::XMVectorSet(x, y, z, w);
}

Vec3f Vec3f::operator+(int other)
{
	return Vec3f(x + other, y + other, z + other);
}

Vec3f Vec3f::operator+(float other)
{
	return Vec3f(x + other, y + other, z + other);
}

Vec3f Vec3f::operator+(Vec3f other)
{
	return Vec3f(x + other.x, y + other.y, z + other.z);
}

Vec3f Vec3f::operator-()
{
	return Vec3f(-x, -y, -z);
}

Vec3f Vec3f::operator-(int other)
{
	return Vec3f(x - other, y - other, z - other);
}

Vec3f Vec3f::operator-(float other)
{
	return Vec3f(x - other, y - other, z - other);
}

Vec3f Vec3f::operator-(Vec3f other)
{
	return Vec3f(x - other.x, y - other.y, z - other.z);
}

Vec3f& Vec3f::operator+=(int other)
{
	x += other;
	y += other;
	z += other;

	return *this;
}

Vec3f& Vec3f::operator+=(float other)
{
	x += other;
	y += other;
	z += other;

	return *this;
}

Vec3f& Vec3f::operator+=(Vec3f other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vec3f& Vec3f::operator-=(int other)
{
	x -= other;
	y -= other;
	z -= other;

	return *this;
}

Vec3f& Vec3f::operator-=(float other)
{
	x -= other;
	y -= other;
	z -= other;

	return *this;
}

Vec3f& Vec3f::operator-=(Vec3f other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vec3f Vec3f::operator*(int other)
{
	return Vec3f(x * other, y * other, z * other);
}

Vec3f Vec3f::operator*(float other)
{
	return Vec3f(x * other, y * other, z * other);
}

Vec3f Vec3f::operator*(double other)
{
	return Vec3f((double)x * other, (double)y * other, (double)z * other);
}

Vec3f Vec3f::operator/(int other)
{
	return Vec3f(x / other, y / other, z / other);
}

Vec3f Vec3f::operator/(float other)
{
	return Vec3f(x / other, y / other, z / other);
}

Vec3f Vec3f::operator/(double other)
{
	return Vec3f(x / other, y / other, z / other);
}

Vec3f& Vec3f::operator*=(int other)
{
	x *= other;
	y *= other;
	z *= other;

	return *this;
}

Vec3f& Vec3f::operator*=(float other)
{
	x *= other;
	y *= other;
	z *= other;

	return *this;
}

Vec3f& Vec3f::operator/=(int other)
{
	x /= other;
	y /= other;
	z /= other;

	return *this;
}

Vec3f& Vec3f::operator/=(float other)
{
	x /= other;
	y /= other;
	z /= other;

	return *this;
}
