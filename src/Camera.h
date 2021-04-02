#pragma once
#include "pch.h"

#include "Vec3.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMMATRIX GetCamera();
	void DrawImGui();
	
	void Translate(Vec3f translation);
	void SetCamera(Vec3f translation);
	void SetFollow(std::function<Vec3f()> func); // pass the postiion function of the object to revolve around
	void LoseFollow(); // stop following an object
	void SetAspectRatio(float aspectratio);
	void SetNearPlane(float nearplane);
	void SetFarPlane(float farplane);
	Vec3f GetPosition();

private:
	Vec3f pos;
	Vec3f dir;
	float r;
	float phi;
	float theta;
	float roll;
	float pitch;
	float yaw;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	bool following;
	std::function<Vec3f()> followFunction;
};

