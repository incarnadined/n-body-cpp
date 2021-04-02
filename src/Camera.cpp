#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: pos(0.0f, 0.0f, -1.0f), dir(0.0f, 0.0f, 1.0f), r(5.0f), phi(0.0f), theta(0.0f), roll(0.0f), pitch(0.0f), yaw(0.0f), fov(pi/2), aspectRatio(16.0f/9.0f), nearPlane(0.5f), farPlane(10.0f),
	  following(false), followFunction(nullptr)
{
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetCamera()
{
	if (following)
	{
		Vec3f pos = followFunction();
		return DirectX::XMMatrixLookAtLH(
				DirectX::XMVector3Transform(
					DirectX::XMVectorSet(pos.GetX(), pos.GetY(), pos.GetZ()-r, 0.0f),
					DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
				), 
				DirectX::XMVectorZero(),
				DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			) 
			* 
			DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
	}

	return	DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookToLH(pos.DX(1.0f), dir.DX(0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) //view
			*
			DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) //view
			*
			DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane) //projection
		);
}

void Camera::DrawImGui()
{
	ImGui::BulletText("Pos X: %f", pos.GetX());
	ImGui::BulletText("Pos Y: %f", pos.GetY());
	ImGui::BulletText("Pos Z: %f", pos.GetZ());
	if (following)
	{
		ImGui::SliderFloat("Distance", &r, 0.0f, 10.0f);
		ImGui::SliderFloat("Phi", &phi, -pi / 2, pi / 2);
		ImGui::SliderFloat("Theta", &theta, -pi, pi);
	}
	ImGui::SliderFloat("Roll", &roll, -pi, pi);
	ImGui::SliderFloat("Pitch", &pitch, -pi/2, pi/2);
	ImGui::SliderFloat("Yaw", &yaw, -pi, pi);
	if (following)
	{
		if (ImGui::Button("Stop following"))
		{
			LoseFollow();
		}
	}
	ImGui::Separator();
}

void Camera::Translate(Vec3f translation)
{
	pos += translation;
}

void Camera::SetCamera(Vec3f translation)
{
	pos = translation;
}

void Camera::SetFollow(std::function<Vec3f()> func)
{
	following = true;
	followFunction = func;
}

void Camera::LoseFollow()
{
	following = false;
	followFunction = nullptr;
}

void Camera::SetAspectRatio(float aspectratio)
{
	aspectRatio = aspectratio;	
}

void Camera::SetNearPlane(float nearplane)
{
	nearPlane = nearplane;	
}

void Camera::SetFarPlane(float farplane)
{
	farPlane = farplane;	
}

Vec3f Camera::GetPosition()
{
	return pos;
}
