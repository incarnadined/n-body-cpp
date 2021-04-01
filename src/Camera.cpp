#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: pos(0.0f, 0.0f, 0.0f), dir(0.0f, 0.0f, 1.0f), roll(0.0f), pitch(0.0f), yaw(0.0f), fov(pi/2), mAspectRatio(16.0f/9.0f), mNearPlane(0.5f), mFarPlane(10.0f)
{
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetCamera()
{
	return	DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) //view
			*
			DirectX::XMMatrixLookToLH(pos.DX(1.0f), dir.DX(0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) //view
			*
			DirectX::XMMatrixPerspectiveFovLH(fov, mAspectRatio, mNearPlane, mFarPlane) //projection
		);
}

void Camera::Translate(Vec3f translation)
{
	pos += translation;
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
