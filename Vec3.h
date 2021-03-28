#pragma once

class Vec3f
{
public:
	Vec3f();
	Vec3f(float x, float y, float z);
	~Vec3f();

	float GetX();
	float GetY();
	float GetZ();

	float magnitude();
	Vec3f normalise();

	// operator overloads
	Vec3f operator+(int other);
	Vec3f operator+(float other);
	Vec3f operator+(Vec3f other);
	Vec3f operator-();
	Vec3f operator-(int other);
	Vec3f operator-(float other);
	Vec3f operator-(Vec3f other);
	Vec3f& operator+=(int other);
	Vec3f& operator+=(float other);
	Vec3f& operator+=(Vec3f other);
	Vec3f& operator-=(int other);
	Vec3f& operator-=(float other);
	Vec3f& operator-=(Vec3f other);
	Vec3f operator*(int other);
	Vec3f operator*(float other);
	Vec3f operator*(double other);
	Vec3f operator/(int other);
	Vec3f operator/(float other);
	Vec3f operator/(double other);
	Vec3f& operator*=(int other);
	Vec3f& operator*=(float other);
	Vec3f& operator/=(int other);
	Vec3f& operator/=(float other);

private:
	float x, y, z;
};

