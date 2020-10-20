#include "PCH.h"
#include "Camera.h"
#include "Core/Application.h"

namespace Evoke
{
	Camera_DEPRECATED::Camera_DEPRECATED() : mProjection(glm::perspectiveFov(mFieldOfView, (f32)Application::Get().MainWindow().Width(), (f32)Application::Get().MainWindow().Height(), mNearClip, mFarClip)),
		mViewProjection(mProjection * mView)
	{
		Application::Get().MainWindow().OnWindowResized.Subscribe([this](u32 inWidth, u32 inHeight)
		{
			Resize(inWidth, inHeight);
		});
	}

	Camera_DEPRECATED::Camera_DEPRECATED(Camera_DEPRECATED&& inCamera) noexcept
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

	void Camera_DEPRECATED::SetFieldOfView(f32 inFoV)
	{
		mFieldOfView = inFoV;
		mProjection = glm::perspectiveFov(mFieldOfView, (f32)Application::Get().MainWindow().Width(), (f32)Application::Get().MainWindow().Height(), mNearClip, mFarClip);
	}

	void Camera_DEPRECATED::Resize(u32 inWidth, u32 inHeight)
	{
		mProjection = glm::perspectiveFov(mFieldOfView, (f32)inWidth, (f32)inHeight, mNearClip, mFarClip);
		mViewProjection = mProjection * mView;
	}

	void Camera_DEPRECATED::RecalculateViewMatrix()
	{
		auto transform = glm::translate(glm::identity<glm::mat4>(), mPosition) * glm::mat4_cast(mRotation);

		mView = glm::inverse(transform);
		mViewProjection = mProjection * mView;
	}

}