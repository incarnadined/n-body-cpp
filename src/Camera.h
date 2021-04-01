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
	
	void SetAspectRatio(float aspectratio);
	void SetNearPlane(float nearplane);
	void SetFarPlane(float farplane);

private:
	Vec3f pos;
	Vec3f dir;
	float roll;
	float pitch;
	float yaw;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};

