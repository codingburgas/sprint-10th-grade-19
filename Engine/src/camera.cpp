#include "camera.h"

Camera::Camera(float clientWidth, float clientHeight, DirectX::SimpleMath::Vector3 initialPosition, float viewDistance)
	: viewDistance(viewDistance)
	, position(initialPosition)
	, yaw(0)
{
	updateView();
	updateAspectRatio(clientWidth, clientHeight);
}

void Camera::updateView()
{
	

	viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(
		position,
		{cosf(yaw), 0, -sinf(yaw)},
		DirectX::SimpleMath::Vector3::Up
	);
}

void Camera::updateAspectRatio(float newWidth, float newHeight)
{
	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XM_PIDIV2, // FOV of 90
		newWidth / newHeight,
		0.1, viewDistance // near and far Z-plane
	);
}

void Camera::adjustPositionAndYaw(const DirectX::SimpleMath::Vector3& posAdjustments, float yawAdjustment)
{
	position += posAdjustments;
	yaw += yawAdjustment;
	updateView();
}

void Camera::adjustYaw(float units)
{
	yaw += units;
	updateView();
}

void Camera::setYaw(float radians)
{
	yaw = radians;
	updateView();
}

void Camera::adjustPosition(const DirectX::SimpleMath::Vector3& adjustments)
{
	position += adjustments;
	updateView();
}

void Camera::setPosition(const DirectX::SimpleMath::Vector3& newPosition)
{
	position = newPosition;
	updateView();
}

DirectX::CXMMATRIX Camera::getViewMatrix() const
{
	return viewMatrix;
}

DirectX::CXMMATRIX Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}
