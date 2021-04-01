#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: pos(0.0f, 0.0f, -1.0f), dir(0.0f, 0.0f, 1.0f), roll(0.0f), pitch(0.0f), yaw(0.0f), fov(pi/2), aspectRatio(16.0f/9.0f), nearPlane(0.5f), farPlane(10.0f)
{
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetCamera()
{
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
	ImGui::SliderFloat("Roll", &roll, -pi, pi);
	ImGui::SliderFloat("Pitch", &pitch, -pi/2, pi/2);
	ImGui::SliderFloat("Yaw", &yaw, -pi, pi);
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
