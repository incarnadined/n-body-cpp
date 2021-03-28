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
	return std::pow(pow(x, 2) + pow(y, 2) + pow(z, 2), 0.5);
}

Vec3f Vec3f::normalise()
{
	return Vec3f(x / magnitude(), y / magnitude(), z / magnitude());
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
