#include "PCH.h"
#include "Scene.h"
#include "Components.h"

namespace Evoke
{

	Scene::Scene() : mMainCamera(CreateEntity("MainCamera")), mActiveCamera(mMainCamera)
	{
		mMainCamera.AddComponent<CameraComponent>();
		mMainCamera.AddComponent<EditorCameraComponent>();
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(StringView inName)
	{
		Entity entity{ mRegistry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<NameComponent>(inName);
		return entity;
	}

	void Scene::Update(f32 inDeltaTime)
	{

	}

	void Scene::PilotEntity(Entity inEntity)
	{
		mActiveCamera = inEntity;
	}

	void Scene::StopPilotingEntity()
	{
		mActiveCamera = mMainCamera;
	}

}