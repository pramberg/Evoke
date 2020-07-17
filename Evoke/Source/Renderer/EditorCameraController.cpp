#include "PCH.h"
#include "EditorCameraController.h"
#include "Core\Application.h"

namespace Evoke
{

	EditorCameraController::EditorCameraController()
	{
		Application::Instance().MainWindow().OnMouseButtonPressed.Subscribe([&](EMouseButton inButton, i32 inRepeat)
		{
			if (inButton == EMouseButton::Left && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mRotationEnabled = true;
				mMouseStartPosition = mMouseCurrentPosition = glm::vec2(Input::MouseX(), Input::MouseY());
				mStartRotation = mCamera.Rotation();
			}

			if (inButton == EMouseButton::Right && Input::IsKeyPressed(EKeyCode::LeftAlt))
			{
				mMouseStartPosition = mMouseCurrentPosition = glm::vec2(Input::MouseX(), Input::MouseY());
			}
		});

		Application::Instance().MainWindow().OnMouseButtonReleased.Subscribe([&](EMouseButton inButton)
		{
			if (inButton == EMouseButton::Left)
			{
				mRotationEnabled = false;
			}
		});

		auto angleNormalized = [](const Vec3f& inV1, const Vec3f& inV2)
		{
			if (Vec3f::Dot(inV1, inV2) > 0.0f)
				return glm::acos(Vec3f::Dot(inV1, inV2));
			else
			{
				glm::vec3 v2n = -inV2;
				return 3.1415f - glm::acos(Vec3f::Dot(inV1, v2n));
			}
		};

		auto interpVec3 = [](const Vec3f& inV1, const Vec3f& inV2, f32 inT)
		{
			const f32 s = 1.0f - inT;
			return inV1 * s + inV2 * inT;
		};

		Application::Instance().MainWindow().OnMouseMoved.Subscribe([&](f32 inX, f32 inY)
		{
			if (mRotationEnabled)
			{
				mMouseCurrentPosition = glm::vec2(inX, inY);

				constexpr f32 sensitivity = glm::radians(0.4f);
				const f32 ratio = (f32)Application::Instance().MainWindow().Width() / (f32)Application::Instance().MainWindow().Height();

				const glm::vec3 up(0.0f, 1.0f, 0.0f);

				const glm::mat3 m = glm::mat3_cast(mStartRotation);
				const glm::mat3 mInv = glm::inverse(m);

				const Vec3f x = mInv[2] - up;
				const f32 lenSqared = Vec3f::Dot(x, x);

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

		Application::Instance().MainWindow().OnMouseScrolled.Subscribe([&](f32 inScrollX, f32 inScrollY)
		{
			const glm::quat invCameraRotation = glm::inverse(mCamera.Rotation());
			const Vec3f forwardVector = (invCameraRotation * Vec3f::Forward()) * -inScrollY * 0.1667f;

			const Vec3f displacement = mCamera.Rotation() * forwardVector;
			mCamera.SetPosition(mCamera.Position() + displacement);
		});
	}

	void EditorCameraController::Update(f32 inDeltaTime)
	{
		const glm::vec2 currentMousePosition(Input::MouseX(), Input::MouseY());
		const glm::vec2 mouseDelta = (currentMousePosition - mMousePreviousPosition);

		const f32 screenWidth = (f32)Application::Instance().MainWindow().Width();
		const f32 screenHeight = (f32)Application::Instance().MainWindow().Height();
		const f32 aspectRatio = screenWidth / screenHeight;

		if (Input::IsKeyPressed(EKeyCode::LeftAlt) && Input::IsMouseButtonPressed(EMouseButton::Middle))
		{
			// #TODO Make movement match 100%, this one is hacky af
			// Look at https://doc.magnum.graphics/magnum/examples-mouseinteraction.html for a real panning implementation
			const f32 zoomLength = glm::length(mCamera.Position());
			const glm::vec2 normalizedDelta = mouseDelta * glm::vec2(1.0f / screenHeight * aspectRatio, 1.0f / screenHeight * aspectRatio) ;
			auto invCameraRotation = glm::inverse(mCamera.Rotation());
			const Vec3f rightVector = (invCameraRotation * Vec3f::Right()) * -normalizedDelta.x  * zoomLength;
			const Vec3f upVector = (invCameraRotation * Vec3f::Up()) * normalizedDelta.y * zoomLength;
			
			const glm::vec3 displacement = mCamera.Rotation() * (rightVector + upVector);
			mCamera.SetPosition(mCamera.Position() + displacement);
		}

		if (Input::IsKeyPressed(EKeyCode::LeftAlt) && Input::IsMouseButtonPressed(EMouseButton::Right))
		{
			constexpr f32 zoomSensitivity = 0.1f;
			f32 zoomLevel = glm::dot(mouseDelta, glm::vec2(-1.0f, 0.0f)) * zoomSensitivity;
			zoomLevel += glm::dot(mouseDelta, glm::vec2(0.0f, 1.0f)) * zoomSensitivity;

			const glm::quat invCameraRotation = glm::inverse(mCamera.Rotation());
			const Vec3f forwardVector = (invCameraRotation * Vec3f::Forward()) * zoomLevel * inDeltaTime;

			const Vec3f displacement = mCamera.Rotation() * forwardVector;
			mCamera.SetPosition(mCamera.Position() + displacement);
		}

		mMousePreviousPosition = currentMousePosition;
	}
}