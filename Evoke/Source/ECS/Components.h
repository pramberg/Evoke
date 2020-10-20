#pragma once
#include "Core/Core.h"
#include "Renderer/Camera.h"

#include "glm/glm.hpp"
#include "entt/entt.hpp"
#include "Core/Application.h"
#include "Assets/AssetRegistry.h"

namespace Evoke
{
	struct NameComponent
	{
		String Name;
	};

	struct SelectionComponent
	{
		b8 Selected;
		i32 SelectionIndex = 0;
	};

	struct TransformComponent
	{
		//glm::mat4 Transform{ 1.0f };

		glm::vec3 Location;
		glm::quat Rotation;
		glm::vec3 RotationE;
		glm::vec3 Scale;

		TransformComponent(const TransformComponent&) = default;
		TransformComponent(
			const glm::vec3& inLocation,
			const glm::quat& inRotation, 
			const glm::vec3& inScale
		) : Location(inLocation), Rotation(inRotation), RotationE(glm::eulerAngles(inRotation)), Scale(inScale) {}

		TransformComponent(
			const glm::vec3& inLocation = glm::vec3{ 0.0f },
			const glm::vec3& inRotation = glm::vec3{ 0.0f },
			const glm::vec3& inScale = glm::vec3{ 1.0f }
		) : Location(inLocation), Rotation(inRotation), RotationE(inRotation), Scale(inScale)
		{
		}

		glm::mat4 CalculateTransform() const
		{
			glm::mat4 transform;
			transform = glm::translate(glm::mat4(1.0f), Location);
			transform = transform * glm::mat4_cast(glm::quat(RotationE));
			transform = glm::scale(transform, Scale);
			return transform;
		}

		operator glm::mat4 () { return CalculateTransform(); }
		operator const glm::mat4 () const { return CalculateTransform(); }
	};

	struct StaticMeshComponent
	{
		entt::id_type MeshId;
#ifdef EV_EDITOR
		i32 CurrentMeshSelection = -1; // Index for GUI
#endif

		StaticMeshComponent() = default;
		StaticMeshComponent(const StaticMeshComponent&) = default;
		StaticMeshComponent(StringView inName, i32 inSelectionIndex = -1) : MeshId(entt::hashed_string::value(inName.data()))
#ifdef EV_EDITOR
			, CurrentMeshSelection(Application::Get().GetAssetRegistry()->GetLoadedMeshSelection(inName)) 
#endif
		{}
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