#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: r(20.0f), theta(0.0f), phi(0.0f), pitch(0.0f), yaw(0.0f), roll(0.0f)
{
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetCamera()
{
	DirectX::XMVECTOR pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);

	return  DirectX::XMMatrixLookAtLH(
				pos, 
				DirectX::XMVectorZero(),
				DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			) 
			* 		
			DirectX::XMMatrixRotationRollPitchYaw(
				pitch, -yaw, roll
			);
}

void Camera::Translate(Vec3f translation)
{
}
