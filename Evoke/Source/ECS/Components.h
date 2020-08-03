#pragma once
#include "Core/Core.h"
#include "Renderer/Camera.h"

#include "glm/glm.hpp"

namespace Evoke
{
	struct NameComponent
	{
		String Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(StringView inName) : Name(inName) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& inTransform) : Transform(inTransform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MeshComponent
	{
		size_t MeshId;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(StringView inName) : MeshId(0) {} // #TODO: Convert string to mesh id
	};

	struct CameraComponent
	{
		Evoke::Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& inProjection) : Camera(inProjection) {}
	};

	struct EditorCameraComponent
	{
	};
}