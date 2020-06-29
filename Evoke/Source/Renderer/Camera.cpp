#include "PCH.h"
#include "Camera.h"
#include "Core/Application.h"

namespace Evoke
{
	Camera::Camera() : mProjection(glm::perspectiveFov(mFieldOfView, (f32)Application::Get().GetWindow().GetWidth(), (f32)Application::Get().GetWindow().GetHeight(), mNearClip, mFarClip)),
		mViewProjection(mProjection * mView)
	{
		Application::Get().GetWindow().OnWindowResized.Subscribe([this](u32 inWidth, u32 inHeight)
		{
			mProjection = glm::perspectiveFov(mFieldOfView, (f32)inWidth, (f32)inHeight, mNearClip, mFarClip);
			mViewProjection = mProjection * mView;
		});
	}

	Camera::Camera(Camera&& inCamera) noexcept
	{
		mView = std::move(inCamera.mView);
		mProjection = std::move(inCamera.mProjection);
		mViewProjection = std::move(inCamera.mViewProjection);
		mPosition = std::move(inCamera.mPosition);
		mRotation = std::move(inCamera.mRotation);
		mFieldOfView = inCamera.mFieldOfView;
		mNearClip = inCamera.mNearClip;
		mFarClip = inCamera.mFarClip;
	}

	void Camera::SetFieldOfView(f32 inFoV)
	{
		mFieldOfView = inFoV;
		mProjection = glm::perspectiveFov(mFieldOfView, (f32)Application::Get().GetWindow().GetWidth(), (f32)Application::Get().GetWindow().GetHeight(), mNearClip, mFarClip);
	}

	void Camera::RecalculateViewMatrix()
	{
		auto transform = glm::translate(glm::identity<glm::mat4>(), -mPosition) * glm::mat4_cast(mRotation);

		mView = transform;// glm::inverse(transform);
		mViewProjection = mProjection * mView;
	}

}