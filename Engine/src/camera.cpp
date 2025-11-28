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
	DirectX::SimpleMath::Vector3 forward = { sinf(yaw), 0, -cosf(yaw) };
	forward.Normalize();

	viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(
		position,
		position + forward,
		DirectX::SimpleMath::Vector3::Up
	);
}

DirectX::SimpleMath::Vector3 Camera::getPos() const
{
	return position;
}

void Camera::updateAspectRatio(float newWidth, float newHeight)
{
	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XM_PIDIV2, // FOV of 90
		newWidth / newHeight,
		0.1, viewDistance // near and far Z-plane
	);
}

void Camera::adjustPositionAndYaw(const DirectX::SimpleMath::Vector3& posAdjustments, int yawAdjustment)
{
	position += posAdjustments;
	yaw += yawAdjustment;
	updateView();
}

void Camera::adjustYaw(int units)
{
	yaw += units * 0.017;
	updateView();
}

void Camera::setYaw(float radians)
{
	yaw = radians;
	updateView();
}

void Camera::adjustPosition(const DirectX::SimpleMath::Vector3& adjustments, bool up)
{
	if (up)
	{
		position.y += 0.0007;
	}
	else
	{
		DirectX::SimpleMath::Vector3 norm = { sinf(yaw), 0, -cosf(yaw) };
		norm.Normalize();
		position += 0.0007 * norm;
	}
	

	
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
	return projectionMatrix.operator DirectX::XMMATRIX();
}
