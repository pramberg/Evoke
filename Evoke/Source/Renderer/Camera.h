#pragma once
#include "Core/Core.h"

namespace Evoke
{
	class Camera_DEPRECATED
	{
	public:
		Camera_DEPRECATED();
		Camera_DEPRECATED(Camera_DEPRECATED&& inCamera) noexcept;

		void SetFieldOfView(f32 inFoV);

		void SetPosition(const glm::vec3& inPosition) { mPosition = inPosition; RecalculateViewMatrix(); }
		const glm::vec3& Position() const { return mPosition; }

		void SetRotation(const glm::quat& inRotation) { mRotation = inRotation; RecalculateViewMatrix(); }
		const glm::quat& Rotation() const { return mRotation; };

		const glm::mat4& Projection() const { return mProjection; }
		const glm::mat4& View() const { return mView; }
		const glm::mat4& ViewProjection() const { return mViewProjection; }

	private:
		void RecalculateViewMatrix();

	private:
		f32 mFieldOfView = glm::radians(90.0f);
		f32 mNearClip = 0.01f;
		f32 mFarClip = 1000.0f;

		glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::quat mRotation = glm::identity<glm::quat>();

		glm::mat4 mView = glm::translate(glm::identity<glm::mat4>(), -mPosition);
		glm::mat4 mProjection;
		glm::mat4 mViewProjection;
	};

	class Camera
	{
	public:
		Camera() : mProjection(1.0f) {}
		Camera(const glm::mat4& inProjection) : mProjection(inProjection) {}
		const glm::mat4& Projection() const { return mProjection; }

	private:
		glm::mat4 mProjection;
	};
}