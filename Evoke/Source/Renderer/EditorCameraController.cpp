#include "PCH.h"
#include "EditorCameraController.h"
#include "Core\Application.h"

#include <imgui.h>
#include "magic_enum.hpp"

namespace Evoke
{

	EditorCameraController::EditorCameraController()
	{
		auto& window = Application::Instance().MainWindow();
		window.OnKeyPressed.Subscribe([&](EKeyCode inButton, i32 inRepeat)
		{
			if (inButton == EKeyCode::F)
			{
				mTarget = glm::vec3{ 0.0f };
				mPosition = (mRotation * glm::vec3{ 0.0f, 0.0f, 1.0f } * mRadius) + mTarget;
				UpdateView();
			}

			if (inButton == EKeyCode::X)
			{
				ResetView();
			}
		});

		window.OnMouseButtonPressed.Subscribe([this](EMouseButton inButton, i32 inRepeat)
		{
			if (Input::IsKeyPressed(EKeyCode::LeftAlt) && (inButton == EMouseButton::Right || inButton == EMouseButton::Middle || inButton == EMouseButton::Left))
				mCameraMode = ECameraMode::Trackball;
			else if (inButton == EMouseButton::Right)
				mCameraMode = ECameraMode::Flycam;
		});

		window.OnMouseButtonReleased.Subscribe([this](EMouseButton inButton)
		{
			if (!Input::IsMouseButtonPressed(EMouseButton::Left) && !Input::IsMouseButtonPressed(EMouseButton::Middle) && !Input::IsMouseButtonPressed(EMouseButton::Right))
				mCameraMode = ECameraMode::None;
		});

		window.OnMouseMoved.Subscribe(EV_BIND(EditorCameraController::MouseMoved));
		window.OnMouseScrolled.Subscribe([this](f32 inScrollX, f32 inScrollY)
		{
			if (mCameraMode == ECameraMode::Flycam)
			{
				mCameraSettings.FlycamSpeed = glm::max(mCameraSettings.FlycamSpeed + inScrollY, 0.01f);
			}
			else
			{
				Zoom(inScrollY * 0.1667f);
				mPosition = (mRotation * glm::vec3{ 0.0f, 0.0f, 1.0f } * mRadius) + mTarget;
				UpdateView();
			}
		});

		Application::Instance().AddOnImGuiRenderCallback([this]()
		{
			static b8 showEditorCameraSettings = true;
			if (!showEditorCameraSettings)
				return;

			ImGui::Begin("Editor Camera Settings", &showEditorCameraSettings);
			ImGui::Text("Current camera mode: %s", magic_enum::enum_name(mCameraMode).data());

			if (ImGui::CollapsingHeader("Flycam"))
			{
				ImGui::DragFloat("Speed", &mCameraSettings.FlycamSpeed, 0.05f, 0.0f, 10.0f, "%.3f", 1.0f);
				ImGui::DragFloat("Rotation Sensitivity", &mCameraSettings.FlycamRotationSensitivity, 0.001f, 0.01f, 1.0f, "%.3f", 1.0f);
			}

			if (ImGui::CollapsingHeader("Trackball"))
			{
				ImGui::DragFloat("Zoom Sensitivity", &mCameraSettings.TrackballZoomSensitivity, 0.001f, 0.0f, 0.1f);
				ImGui::DragFloat("Pan Sensitivity", &mCameraSettings.TrackballPanSensitivity, 0.001f, 0.0f, 0.1f);
				ImGui::DragFloat("Rotation Sensitivity", &mCameraSettings.TrackballRotationSensitivity, 0.001f, 0.01f, 1.0f);
			}
			ImGui::End();
		});

		ResetView();
	}

	void EditorCameraController::Update(f32 inDeltaTime)
	{
		const glm::vec2 currentMousePosition{ Input::MouseX(), Input::MouseY() };
		mMouseDelta = currentMousePosition - mMousePreviousPosition;
		mMousePreviousPosition = currentMousePosition;

		if (mCameraMode == ECameraMode::Flycam)
		{
			glm::vec3 displacement{ 0.0f };
			auto accumulateDisplacement = [this, &displacement, inDeltaTime](const glm::vec3& inVector)
			{
				displacement += mRotation * inVector * inDeltaTime * mCameraSettings.FlycamSpeed;
			};

			if (Input::IsKeyPressed(EKeyCode::W)) { accumulateDisplacement({ 0.0f,  0.0f, -1.0f }); }
			if (Input::IsKeyPressed(EKeyCode::S)) { accumulateDisplacement({ 0.0f,  0.0f,  1.0f }); }
			if (Input::IsKeyPressed(EKeyCode::D)) { accumulateDisplacement({ 1.0f,  0.0f,  0.0f }); }
			if (Input::IsKeyPressed(EKeyCode::A)) { accumulateDisplacement({-1.0f,  0.0f,  0.0f }); }
			if (Input::IsKeyPressed(EKeyCode::E)) { accumulateDisplacement({ 0.0f,  1.0f,  0.0f }); }
			if (Input::IsKeyPressed(EKeyCode::Q)) { accumulateDisplacement({ 0.0f, -1.0f,  0.0f }); }

			mPosition += displacement;
			mTarget += displacement;
			UpdateView();
		}

		if (mCameraMode == ECameraMode::None)
		{
			Application::Instance().MainWindow().SetCursorMode(ECursorMode::Normal);
		}
	}

	void EditorCameraController::Zoom(f32 inDistance)
	{
		mRadius -= inDistance;

		if (mRadius <= 0.0f)
		{
			mRadius = 30.0f;
			auto look = mRotation * glm::vec3{ 0.0f, 0.0f, 1.0f };
			mTarget = mTarget - look * 30.0f;
		}
	}

	void EditorCameraController::Rotate(f32 inDeltaYaw, f32 inDeltaPitch)
	{
		mYaw += inDeltaYaw;
		mPitch += inDeltaPitch;

		const glm::quat rotationX = glm::angleAxis(mPitch, glm::vec3{1.0f, 0.0f, 0.0f});
		const glm::quat rotationY = glm::angleAxis(mYaw, glm::vec3{ 0.0f, 1.0f, 0.0f });

		mRotation = glm::normalize(rotationY * rotationX);
	}

	void EditorCameraController::Pan(f32 inDeltaX, f32 inDeltaY)
	{
		const glm::vec3 right = mRotation * glm::vec3{ -1.0f, 0.0f, 0.0f };
		const glm::vec3 up = mRotation * glm::vec3{ 0.0f, 1.0f, 0.0f };

		mTarget += (right * inDeltaX) + (up * inDeltaY);
	}

	void EditorCameraController::MouseMoved(f32 inMouseX, f32 inMouseY)
	{
		b8 shouldUpdate = false;
		if (mCameraMode == ECameraMode::Trackball)
		{
			if (Input::IsMouseButtonPressed(EMouseButton::Left))
			{
				shouldUpdate = true;
				const auto localDelta = -mMouseDelta * glm::radians(mCameraSettings.TrackballRotationSensitivity);
				Rotate(localDelta.x, localDelta.y);
			}
			else if (Input::IsMouseButtonPressed(EMouseButton::Middle))
			{
				shouldUpdate = true;
				const auto localDelta = mMouseDelta * mCameraSettings.TrackballPanSensitivity;
				Pan(localDelta.x, localDelta.y);
			}
			else if (Input::IsMouseButtonPressed(EMouseButton::Right))
			{
				shouldUpdate = true;
				const f32 zoomLevel = mMouseDelta.x - mMouseDelta.y;
				Zoom(zoomLevel * mRadius * mCameraSettings.TrackballZoomSensitivity);
			}
		}
		else if (mCameraMode == ECameraMode::Flycam)
		{
			Application::Instance().MainWindow().SetCursorMode(ECursorMode::Disabled);

			const auto localDelta = -mMouseDelta * glm::radians(mCameraSettings.FlycamRotationSensitivity);
			Rotate(localDelta.x, localDelta.y);

			mTarget = mPosition - (mRotation * glm::vec3{ 0.0f, 0.0f, 1.0f } * mRadius);
			shouldUpdate = true;
		}

		if (shouldUpdate)
		{
			if (mCameraMode == ECameraMode::Trackball) { mPosition = (mRotation * glm::vec3{ 0.0f, 0.0f, 1.0f } * mRadius) + mTarget; }
			UpdateView();
		}

	}

	void EditorCameraController::UpdateView()
	{
		auto mat = glm::translate(glm::identity<glm::mat4>(), mPosition) * glm::mat4_cast(mRotation);
		mCamera.SetView(glm::inverse(mat));
	}

	void EditorCameraController::ResetView()
	{
		mTarget = glm::vec3{ 0.0f };

		mPosition = glm::vec3{ 3.0f, 2.0f, 3.0f };
		mRadius = glm::length(mPosition);

		mRotation = glm::quatLookAt(glm::normalize(-mPosition), glm::vec3{ 0.0f, 1.0f, 0.0f });

		glm::vec3 cameraForward = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
		mYaw = glm::atan(cameraForward.z, cameraForward.x);
		mPitch = -glm::asin(cameraForward.y);

		UpdateView();
	}

}