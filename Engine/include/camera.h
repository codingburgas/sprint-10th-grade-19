#include <SimpleMath.h>

class Camera
{
public:
	Camera(float clientWidth, float clientHeight, float viewDistance = 10);

	DirectX::CXMMATRIX getViewMatrix() const;
	DirectX::CXMMATRIX getProjectionMatrix() const;

	void updateAspectRatio(float newWidth, float newHeight);

private:
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projectionMatrix;

	float viewDistance;
};
