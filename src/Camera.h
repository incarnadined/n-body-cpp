#pragma once
#include "pch.h"

#include "Vec3.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMMATRIX GetCamera();
	void Translate(Vec3f translation);

private:
	float r;
	float theta;
	float phi;
	float pitch;
	float yaw;
	float roll;
};

