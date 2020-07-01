#include "PCH.h"
#include "EditorCameraController.h"
#include "Core\Application.h"

namespace Evoke
{

	EditorCameraController::EditorCameraController()
	{
		Application::Get().GetWindow().OnMouseButtonPressed.Subscribe([&](EMouseButton inButton, i32 inRepeat)
		{
			if (inButton == EMouseButton::Left && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mRotationEnabled = true;
				mMouseStartPosition = mMouseCurrentPosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
				mStartRotation = mCamera.GetRotation();
			}

			if (inButton == EMouseButton::Right && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mMouseStartPosition = mMouseCurrentPosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
			}
		});

		Application::Get().GetWindow().OnMouseButtonReleased.Subscribe([&](EMouseButton inButton)
		{
			if (inButton == EMouseButton::Left)
			{
				mRotationEnabled = false;
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
			if (mRotationEnabled)
			{
				mMouseCurrentPosition = glm::vec2(inX, inY);

				constexpr f32 sensitivity = glm::radians(0.4f);
				const f32 ratio = (f32)Application::Get().GetWindow().GetWidth() / (f32)Application::Get().GetWindow().GetHeight();

				const glm::vec3 up(0.0f, 1.0f, 0.0f);

				const glm::mat3 m = glm::mat3_cast(mStartRotation);
				const glm::mat3 mInv = glm::inverse(m);

				const glm::vec3 x = mInv[2] - up;
				const f32 lenSqared = glm::dot(x, x);

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

				const glm::quat localX = mStartRotation * glm::angleAxis(sensitivity * (mMouseCurrentPosition.y - mMouseStartPosition.y), xAxis);
				const glm::quat globalUp = glm::angleAxis(sensitivity * (mMouseCurrentPosition.x - mMouseStartPosition.x) * ratio, up);
				mCamera.SetRotation(glm::normalize(localX * globalUp));
			}
		});

		Application::Get().GetWindow().OnMouseScrolled.Subscribe([&](f32 inScrollX, f32 inScrollY)
		{
			const glm::quat invCameraRotation = glm::inverse(mCamera.GetRotation());
			const glm::vec3 forwardVector = (invCameraRotation * glm::vec3(0.0f, 0.0f, 1.0f)) * -inScrollY * 0.1667f;

			const glm::vec3 displacement = mCamera.GetRotation() * forwardVector;
			mCamera.SetPosition(mCamera.GetPosition() + displacement);
		});
	}

	void EditorCameraController::Update(f32 inDeltaTime)
	{
		const glm::vec2 currentMousePosition(Input::GetMouseX(), Input::GetMouseY());
		const glm::vec2 mouseDelta = (currentMousePosition - mMousePreviousPosition);

		const f32 screenWidth = (f32)Application::Get().GetWindow().GetWidth();
		const f32 screenHeight = (f32)Application::Get().GetWindow().GetHeight();
		const f32 aspectRatio = screenWidth / screenHeight;

		if (Input::IsKeyPressed(EKeyCode::LeftAlt) && Input::IsMouseButtonPressed(EMouseButton::Middle))
		{
			// #TODO Make movement match 100%, this one is hacky af
			// Look at https://doc.magnum.graphics/magnum/examples-mouseinteraction.html for a real panning implementation
			const f32 zoomLength = glm::length(mCamera.GetPosition());
			const glm::vec2 normalizedDelta = mouseDelta * glm::vec2(1.0f / screenHeight * aspectRatio, 1.0f / screenHeight * aspectRatio) ;
			auto invCameraRotation = glm::inverse(mCamera.GetRotation());
			const glm::vec3 rightVector = (invCameraRotation * Vec3f::Right()) * -normalizedDelta.x  * zoomLength;
			const glm::vec3 upVector = (invCameraRotation * Vec3f::Up()) * normalizedDelta.y * zoomLength;
			
			const glm::vec3 displacement = mCamera.GetRotation() * (rightVector + upVector);
			mCamera.SetPosition(mCamera.GetPosition() + displacement);
		}

		if (Input::IsKeyPressed(EKeyCode::LeftAlt) && Input::IsMouseButtonPressed(EMouseButton::Right))
		{
			constexpr f32 zoomSensitivity = 0.1f;
			f32 zoomLevel = glm::dot(mouseDelta, glm::vec2(-1.0f, 0.0f)) * zoomSensitivity;
			zoomLevel += glm::dot(mouseDelta, glm::vec2(0.0f, 1.0f)) * zoomSensitivity;

			const glm::quat invCameraRotation = glm::inverse(mCamera.GetRotation());
			const glm::vec3 forwardVector = (invCameraRotation * glm::vec3(0.0f, 0.0f, 1.0f)) * zoomLevel * inDeltaTime;

			const glm::vec3 displacement = mCamera.GetRotation() * forwardVector;
			mCamera.SetPosition(mCamera.GetPosition() + displacement);
		}

		mMousePreviousPosition = currentMousePosition;
	}
}