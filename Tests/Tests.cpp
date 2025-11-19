#include "CppUnitTest.h"
#include "camera.h"
#include <SimpleMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CameraTest)
	{
	public:
		Camera camera;
		float testRotation = 1;
		DirectX::SimpleMath::Matrix testMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(testRotation);
		
		TEST_METHOD(YawRotation)
		{
			camera.setPosition({0, 0, 0});
			camera.setYaw(testRotation);
			Assert::AreEqual(camera.getViewMatrix(), static_cast<DirectX::CXMMATRIX>(testMatrix));
		}
	};
}
