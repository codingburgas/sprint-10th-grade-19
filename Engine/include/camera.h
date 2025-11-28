#pragma once

#include <SimpleMath.h>

// Provides view and projection matrices for drawing objects relative to a point.
// Allows changing the position and the Y-axis rotation.
class Camera
{
public:
	Camera(
		float clientWidth = 16,
		float clientHeight = 9,
		DirectX::SimpleMath::Vector3 initialPosition = DirectX::SimpleMath::Vector3::Zero,
		float viewDistance = 100
	);

	void adjustPositionAndYaw(const DirectX::SimpleMath::Vector3& posAdjustments, int yawAdjustment);

	void adjustPosition(const DirectX::SimpleMath::Vector3& adjustments, bool up);
	void setPosition(const DirectX::SimpleMath::Vector3& newPosition);

	void adjustYaw(int units);
	void setYaw(float radians);

	DirectX::CXMMATRIX getViewMatrix() const;
	DirectX::CXMMATRIX getProjectionMatrix() const;

	// Should be called when the window is resized.
	void updateAspectRatio(float newWidth, float newHeight);
	DirectX::SimpleMath::Vector3 getPos() const;
private:
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projectionMatrix;
	float viewDistance;

	DirectX::SimpleMath::Vector3 position;
	float yaw;

	void updateView();
};
