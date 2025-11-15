#include "camera.h"

Camera::Camera(float clientWidth, float clientHeight, float viewDistance/*=10*/)
	: viewDistance(viewDistance)
{
	viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(
		DirectX::SimpleMath::Vector3::Zero, // initial camera position
		DirectX::SimpleMath::Vector3::Forward, // look at position
		DirectX::SimpleMath::Vector3::Up // up vector
	);

	updateAspectRatio(clientWidth, clientHeight);
}

void Camera::updateAspectRatio(float newWidth, float newHeight)
{
	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XM_PIDIV2, // FOV of 90
		newWidth / newHeight,
		0.1, viewDistance // near and far Z-plane
	);
}

DirectX::CXMMATRIX Camera::getViewMatrix() const
{
	return viewMatrix;
}

DirectX::CXMMATRIX Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}
