#pragma once
#include "Core/Core.h"
#include "Core/Input.h"
#include "Camera.h"

namespace Evoke
{
	struct CameraSettings
	{
		f32 FlycamSpeed = 1.0f;
		f32 FlycamRotationSensitivity = 0.2f;
		f32 TrackballZoomSensitivity = 0.01f;
		f32 TrackballPanSensitivity = 0.02f;
		f32 TrackballRotationSensitivity = 0.2f;
	};

	class EditorCameraController
	{
	public:
		EditorCameraController();
		void Update(f32 inDeltaTime);

		class Camera_DEPRECATED& Camera() { return mCamera; }
		const class Camera_DEPRECATED& Camera() const { return mCamera; }
	private:
		void Zoom(f32 inDistance);
		void Rotate(f32 inDeltaTheta, f32 inDeltaPhi);
		void Pan(f32 inDeltaX, f32 inDeltaY);
		void MouseMoved(f32 inMouseX, f32 inMouseY);

		void UpdateView();
		void ResetView();

		class Camera_DEPRECATED mCamera;
		CameraSettings mCameraSettings;

		enum class ECameraMode : i8
		{
			Flycam,
			Trackball,
			None,
			Count,
		} mCameraMode = ECameraMode::None;

		glm::vec2 mMousePreviousPosition = glm::vec2(0.0f);
		glm::vec2 mMouseDelta = glm::vec2(0.0f);

		glm::quat mRotation = glm::identity<glm::quat>();
		glm::vec3 mPosition{ 0.0f };
		glm::vec3 mTarget{ 0.0f };

		f32 mYaw = 0.0f;
		f32 mPitch = 0.0f;
		f32 mRadius = 5.0f;;

	};
}