#pragma once
#include "Core/Core.h"
#include "Core/Input.h"
#include "Camera.h"

namespace Evoke
{
	class OrbitCameraController
	{
	public:
		OrbitCameraController();
		void Update(f32 inDeltaTime);

		Camera& GetCamera() { return mCamera; }
	private:
		Camera mCamera;
		glm::vec3 mTargetPosition = glm::vec3(0.0f);
		glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec2 mPreviousMouse = glm::vec2(0.0f);
		glm::vec2 mCurrentMouse = glm::vec2(0.0f);
		b8 mArcballEnabled = false;

		glm::quat mStartRotation = glm::identity<glm::quat>();
		glm::vec3 mStartPosition = glm::vec3(0.0f);
		glm::vec2 mStartMouse = glm::vec2(0.0f);
	};
}