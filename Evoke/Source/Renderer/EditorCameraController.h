#pragma once
#include "Core/Core.h"
#include "Core/Input.h"
#include "Camera.h"

namespace Evoke
{
	class EditorCameraController
	{
	public:
		EditorCameraController();
		void Update(f32 inDeltaTime);

		Camera& GetCamera() { return mCamera; }
	private:
		Camera mCamera;

		b8 mRotationEnabled = false;
		glm::vec2 mMouseStartPosition = glm::vec2(0.0f);
		glm::vec2 mMousePreviousPosition = glm::vec2(0.0f);
		glm::vec2 mMouseCurrentPosition = glm::vec2(0.0f);
		glm::quat mStartRotation = glm::identity<glm::quat>();
	};
}