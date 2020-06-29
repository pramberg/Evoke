#include "PCH.h"
#include "OrbitCameraController.h"
#include "Core\Application.h"

namespace Evoke
{

	OrbitCameraController::OrbitCameraController()
	{
		Application::Get().GetWindow().OnMouseButtonPressed.Subscribe([&](EMouseButton inButton, i32 inRepeat)
		{
			if (inButton == EMouseButton::Left && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mArcballEnabled = true;
				mStartMouse = mCurrentMouse = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
				mStartRotation = mCamera.GetRotation();
			}

			if (inButton == EMouseButton::Middle && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mStartPosition = mCamera.GetPosition();
			}

			if (inButton == EMouseButton::Right && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mStartMouse = mCurrentMouse = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
			}
		});

		Application::Get().GetWindow().OnMouseButtonReleased.Subscribe([&](EMouseButton inButton)
		{
			if (inButton == EMouseButton::Left)
			{
				mArcballEnabled = false;
			}
		});

		auto angleNormalized = [](const glm::vec3& inV1, const glm::vec3& inV2)
		{
			if (glm::dot(inV1, inV2) > 0.0f)
				return glm::acos(glm::dot(inV1, inV2));
			else
			{
				glm::vec3 v2n = -inV2;
				return 3.1415f - glm::acos(glm::dot(inV1, v2n));
			}
		};

		auto interpVec3 = [](const glm::vec3& inV1, const glm::vec3 inV2, f32 inT)
		{
			const f32 s = 1.0f - inT;
			return inV1 * s + inV2 * inT;
		};

		Application::Get().GetWindow().OnMouseMoved.Subscribe([&](f32 inX, f32 inY)
		{
			if (mArcballEnabled)
			{
				mCurrentMouse = glm::vec2(inX, inY);

				constexpr f32 sensitivity = glm::radians(1.0f);
				const f32 ratio = (f32)Application::Get().GetWindow().GetWidth() / (f32)Application::Get().GetWindow().GetHeight();

				const glm::vec3 up(0.0f, 1.0f, 0.0f);

				glm::mat3 m = glm::mat3_cast(mStartRotation);
				glm::mat3 mInv = glm::inverse(m);

				glm::vec3 x = mInv[2] - up;
				f32 lenSqared = glm::dot(x, x);

				glm::vec3 xAxis;
				if (lenSqared > 0.001f)
				{
					f32 fac;
					xAxis = glm::cross(up, mInv[2]);
					if (glm::dot(xAxis, mInv[0]) < 0.0f)
						xAxis = -xAxis;

					fac = angleNormalized(up, mInv[2]) / (f32)Math::Pi;
					fac = glm::abs(fac - 0.5f) * 2.0f;
					fac = fac * fac;
					xAxis = interpVec3(xAxis, mInv[0], fac);
				}
				else
				{
					xAxis = mInv[0];
				}

				auto localX = glm::angleAxis(sensitivity * (mCurrentMouse.y - mStartMouse.y), xAxis);
				localX = mStartRotation * localX;

				auto globalUp = glm::angleAxis(sensitivity * (mCurrentMouse.x - mStartMouse.x) * ratio, up);
				auto finalRot = glm::normalize(localX * globalUp);
				mCamera.SetRotation(finalRot);
			}
		});

		Application::Get().GetWindow().OnMouseScrolled.Subscribe([&](f32 inScrollX, f32 inScrollY)
		{
			auto invCameraRotation = glm::inverse(mCamera.GetRotation());
			const glm::vec3 forwardVector = (invCameraRotation * glm::vec3(0.0f, 0.0f, 1.0f)) * -inScrollY * 0.1667f;

			const glm::vec3 displacement = mCamera.GetRotation() * forwardVector;
			mCamera.SetPosition(mCamera.GetPosition() + displacement);
		});
	}

	void OrbitCameraController::Update(f32 inDeltaTime)
	{
		glm::vec2 currentMousePosition(Input::GetMouseX(), Input::GetMouseY());
		glm::vec2 mouseDelta = (currentMousePosition - mPreviousMouse);

		const f32 screenWidth = (f32)Application::Get().GetWindow().GetWidth();
		const f32 screenHeight = (f32)Application::Get().GetWindow().GetHeight();
		const f32 aspectRatio = screenWidth / screenHeight;
		const f32 aspectRatio2 = screenHeight / screenWidth;

		if (Input::IsKeyPressed(EKeyCode::LeftAlt) && Input::IsMouseButtonPressed(EMouseButton::Middle))
		{
			const f32 zoomLength = glm::length(mCamera.GetPosition());
			const glm::vec2 normalizedDelta = mouseDelta * glm::vec2(1.0f / screenHeight * aspectRatio, 1.0f / screenHeight * aspectRatio) ;
			auto invCameraRotation = glm::inverse(mCamera.GetRotation());
			const glm::vec3 rightVector = (invCameraRotation * glm::vec3(1.0f, 0.0f, 0.0f)) * -normalizedDelta.x  * zoomLength;
			const glm::vec3 upVector = (invCameraRotation * glm::vec3(0.0f, 1.0f, 0.0f)) * normalizedDelta.y * zoomLength; // #TODO How to make a movement match 100%?

			const glm::vec3 displacement = mCamera.GetRotation() * (rightVector + upVector);
			mCamera.SetPosition(mCamera.GetPosition() + displacement);
		}

		if (Input::IsKeyPressed(EKeyCode::LeftAlt) && Input::IsMouseButtonPressed(EMouseButton::Right))
		{
			constexpr f32 zoomSensitivity = 0.1f;
			f32 zoomLevel = glm::dot(mouseDelta, glm::vec2(-1.0f, 0.0f)) * zoomSensitivity;
			zoomLevel += glm::dot(mouseDelta, glm::vec2(0.0f, 1.0f)) * zoomSensitivity;

			auto invCameraRotation = glm::inverse(mCamera.GetRotation());
			const glm::vec3 forwardVector = (invCameraRotation * glm::vec3(0.0f, 0.0f, 1.0f)) * zoomLevel * 0.1667f;

			const glm::vec3 displacement = mCamera.GetRotation() * forwardVector;
			mCamera.SetPosition(mCamera.GetPosition() + displacement);
		}

		mPreviousMouse = currentMousePosition;
	}
}