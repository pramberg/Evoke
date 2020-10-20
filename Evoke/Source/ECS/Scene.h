#pragma once
#include "Core/Core.h"
#include "Entity.h"
#include "Renderer/GraphicsContext.h"

#include "entt/entt.hpp"

namespace Evoke
{
	class StaticMesh;
	class Texture2D;
	class Sampler;
	class Material;
	class Outliner;

	template<typename T>
	class TVertexBuffer;

	template<typename T>
	class TIndexBuffer;

	constexpr u32 NumVertexColors = 2;
	constexpr u32 NumUVSets = 6;

	struct SimpleVertex
	{
		glm::vec3 Position;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(StringView inName = "New Entity");
		void Update(f32 inDeltaTime, GraphicsContext* inOutGraphicsContext);

		void PilotEntity(Entity inEntity);
		void StopPilotingEntity();

		void SetSelection(const std::vector<entt::entity>& inSelection);
		void SetSelection(std::vector<entt::entity>&& inSelection);
		void AddSelection(entt::entity inEntity);
		void RemoveSelection(entt::entity inEntity);
		void ToggleSelection(entt::entity inEntity);
		b8 IsSelected(entt::entity inEntity);
		
		/**
		 * Extends the current selection to the next entity (typical shift click behavior)
		 * @param 	inEntity	The entity.
		 */
		void ExtendSelection(entt::entity inEntity);
		//std::vector<entt::entity> Selection();
		std::vector<entt::entity>& Selection() { return mSelection; }
		const std::vector<entt::entity>& Selection() const { return mSelection; }

		const entt::registry& Registry() const { return mRegistry; }
		entt::registry& Registry() { return mRegistry; }

	private:
		entt::registry mRegistry;
		std::vector<entt::entity> mSelection;
		// Gets set both when selecting and deselecting entities
		entt::entity mLastInteractedEntity;

		Entity mMainCamera;
		Entity mActiveCamera;
		entt::entity mCurrentRenameEntity = entt::null;

		// #TEMP
		TSharedPtr<Texture2D> mTexture1;
		TSharedPtr<Texture2D> mTexture2;
		TSharedPtr<Sampler> mSharedSampler;

		friend class Entity;
		Material* mSkyboxMaterial;
		Material* mLineMaterial;
		TSharedPtr<Texture2D> mSkyboxTexture;
		TUniquePtr<TVertexBuffer<std::array<SimpleVertex, 8>>> mCubeVBO;
		TUniquePtr<TIndexBuffer<std::array<u32, 36>>> mCubeEBO;
		TUniquePtr<TVertexBuffer<std::array<SimpleVertex, 4>>> mGridVBO;
		TUniquePtr<TIndexBuffer<std::array<u32, 6>>> mGridEBO;
	};
}