#include "PCH.h"
#include "Scene.h"
#include "Components.h"
#include "Platform\Generic\Dialog.h"
#include "Debug\Widgets\Outliner.h"

#include "Renderer/StaticMesh.h"
#include "Core\Application.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Assets\AssetRegistry.h"

#include "Renderer\Slots\SlotRenderer.h"

namespace Evoke
{
	constexpr std::pair<std::array<SimpleVertex, 8>, std::array<u32, 36>> MakeSimpleCube()
	{
		constexpr std::array<SimpleVertex, 8> vertices
		{
			SimpleVertex{ glm::vec3{-1.0f, -1.0f, -1.0f} },
			SimpleVertex{ glm::vec3{1.0f, -1.0f, -1.0f} },
			SimpleVertex{ glm::vec3{1.0f, 1.0f, -1.0f} },
			SimpleVertex{ glm::vec3{-1.0f, 1.0f, -1.0f} },
			SimpleVertex{ glm::vec3{-1.0f, 1.0f, 1.0f} },
			SimpleVertex{ glm::vec3{1.0f, 1.0f, 1.0f} },
			SimpleVertex{ glm::vec3{1.0f, -1.0f, 1.0f} },
			SimpleVertex{ glm::vec3{-1.0f, -1.0f, 1.0f} },
		};
		constexpr std::array<u32, 36> indices
		{
			0, 2, 1,
			0, 3, 2,
			2, 3, 4,
			2, 4, 5,
			1, 2, 5,
			1, 5, 6,
			0, 7, 4,
			0, 4, 3,
			5, 4, 7,
			5, 7, 6,
			0, 6, 7,
			0, 1, 6
		};
		return std::make_pair(vertices, indices);
	}

	template<u32 X, u32 Z = X>
	constexpr std::pair<std::array<SimpleVertex, (X + 1)* (Z + 1)>, std::array<u32, X* Z * 6>> MakeSimpleGrid()
	{
		std::array<SimpleVertex, (X + 1)* (Z + 1)> vertices;
		u32 c = 0;
		for (u32 j = 0; j <= Z; j++)
		{
			for (u32 i = 0; i <= X; i++)
			{
				f32 x = (f32)i / (f32)X - 0.5f;
				f32 z = (f32)j / (f32)Z - 0.5f;
				vertices[c++] = { glm::vec3{ x, 0.0f, z } };
			}
		}

		std::array<u32, X* Z * 6> indices;
		for (u32 ti = 0, vi = 0, z = 0; z < Z; z++, vi++)
		{
			for (u32 x = 0; x < X; x++, ti += 6, vi++)
			{
				indices[ti] = vi;
				indices[ti + 3LL] = indices[ti + 2LL] = vi + 1;
				indices[ti + 4LL] = indices[ti + 1LL] = vi + X + 1;
				indices[ti + 5LL] = vi + X + 2;
			}
		}

		return std::make_pair(vertices, indices);
	}


	Scene::Scene() : mMainCamera(CreateEntity("MainCamera")), mActiveCamera(mMainCamera)
	{
		/*SlotRenderer::Slot basePass{
			"BasePass",
			{
				[](GraphicsContext* inContext)
				{
				
				},
				[](GraphicsContext* inContext)
				{

				}
			}
		};
		SlotRenderer renderer{ basePass };*/

		mMainCamera.AddComponent<CameraComponent>();
		mMainCamera.AddComponent<EditorCameraComponent>();

		mSharedSampler = Sampler::Create({ ETextureAddressMode::Wrap, ETextureFilter::Anisotropic, 16 });

		mTexture1 = Texture2D::Create("Assets/TestTexture1.png");
		mTexture1->SetSampler(mSharedSampler);
		mTexture2 = Texture2D::Create("Assets/TestTexture2.png");
		mTexture2->SetSampler(mSharedSampler);

		ShaderCompilerConfig cfg;
		cfg.AddDefine("NUM_VERTEX_COLORS", std::to_string(2));
		cfg.AddDefine("NUM_UV_SETS", std::to_string(6));

		auto defaultMaterial = new Material("../Shaders/TestShader.hlsl", cfg);
		defaultMaterial->SetTextureParameter(0, mTexture1.get());
		defaultMaterial->SetTextureParameter(1, mTexture2.get());

		mSkyboxMaterial = new Material("../Shaders/Skybox.hlsl");
		mLineMaterial = new Material("../Shaders/EditorGrid.hlsl");
		mLineMaterial->SetParameter(0, glm::vec3{ 0.4f, 0.4f, 0.4f });

		auto anisotropicClampSampler = Sampler::Create({ ETextureAddressMode::Clamp, ETextureFilter::Anisotropic, 16 });
		auto linearWrapSampler = Sampler::Create({ ETextureAddressMode::Wrap, ETextureFilter::Linear, 1 });

		TextureDescription skyDesc{ ETextureFormat::R16G16B16A16_Float };
		mSkyboxTexture = Texture2D::Create("Assets/evening_road_01_2k.hdr", skyDesc);
		mSkyboxTexture->SetSampler(linearWrapSampler);
		mSkyboxMaterial->SetTextureParameter(0, mSkyboxTexture.get());

		auto [cubeVertices, cubeIndices] = MakeSimpleCube();
		auto [gridVertices, gridIndices] = MakeSimpleGrid<1>();

		mCubeVBO = VertexBuffer::Create(cubeVertices);
		mCubeEBO = IndexBuffer::Create(cubeIndices);

		mGridVBO = VertexBuffer::Create(gridVertices);
		mGridEBO = IndexBuffer::Create(gridIndices);

		Application::Get().MainWindow().OnKeyReleased.Subscribe([&](EKeyCode inKey)
		{
			if (inKey == EKeyCode::F2)
			{
				auto sel = Selection();
				if (!sel.empty() && mCurrentRenameEntity != sel[0])
					mCurrentRenameEntity = sel[0];
				else
					mCurrentRenameEntity = entt::null;
			}
		});

		AssetRegistry* ar = Application::Get().GetAssetRegistry();
		auto& rc = ar->GetResourceCache();
		rc.each([defaultMaterial](entt::resource_handle<StaticMesh> inMesh)
		{
			if (!inMesh)
				return;

			inMesh->SetMaterial(defaultMaterial);
		});
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(StringView inName)
	{
		Entity entity{ mRegistry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<NameComponent>(inName.data());
		entity.AddComponent<SelectionComponent>(false);
		SetSelection({ entity.ID() });
		return entity;
	}

	void Scene::Update(f32 inDeltaTime, GraphicsContext* inOutGraphicsContext)
	{
		auto viewData = ConstantBuffer::Create<glm::mat4>(1);
		AssetRegistry* ar = Application::Get().GetAssetRegistry();
		auto& rc = ar->GetResourceCache();

		auto group = mRegistry.group<TransformComponent>(entt::get<StaticMeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, StaticMeshComponent>(entity);

			auto hdl = rc.handle(mesh.MeshId);
			if (hdl)
			{
				hdl->GetMaterial()->SetParameter(0, transform.CalculateTransform());
				hdl->Render(inOutGraphicsContext);
			}
		}


		inOutGraphicsContext->BeginEvent("Skybox");
		inOutGraphicsContext->SetFaceCullingMethod(EFaceCulling::Front);
		mSkyboxMaterial->Bind();
		mSkyboxTexture->Bind(0);
		mCubeVBO->Bind();
		inOutGraphicsContext->DrawIndexed(mCubeEBO->Size());
		inOutGraphicsContext->EndEvent();

		// Transparent - must be rendered last
		inOutGraphicsContext->BeginEvent("Grid");
		mLineMaterial->Bind();
		mGridVBO->Bind();
		inOutGraphicsContext->SetFaceCullingMethod(EFaceCulling::Disabled);
		inOutGraphicsContext->DrawIndexed(mGridEBO->Size());
		inOutGraphicsContext->EndEvent();
	}

	void Scene::PilotEntity(Entity inEntity)
	{
		mActiveCamera = inEntity;
	}

	void Scene::StopPilotingEntity()
	{
		mActiveCamera = mMainCamera;
	}

	void Scene::SetSelection(const std::vector<entt::entity>& inSelection)
	{
		mSelection = inSelection;
		if (!mSelection.empty())
			mLastInteractedEntity = mSelection[mSelection.size() - 1];
	}

	void Scene::SetSelection(std::vector<entt::entity>&& inSelection)
	{
		mSelection = inSelection;
		if (!mSelection.empty())
			mLastInteractedEntity = mSelection[mSelection.size() - 1];
	}

	void Scene::AddSelection(entt::entity inEntity)
	{
		if (!IsSelected(inEntity))
		{
			mSelection.push_back(inEntity);
			mLastInteractedEntity = inEntity;
		}
	}

	void Scene::RemoveSelection(entt::entity inEntity)
	{
		auto selectionIt = std::find(mSelection.begin(), mSelection.end(), inEntity);
		if (selectionIt != mSelection.end())
		{
			mSelection.erase(selectionIt);
			mLastInteractedEntity = inEntity;
		}
	}

	void Scene::ToggleSelection(entt::entity inEntity)
	{
		auto selectionIt = std::find(begin(mSelection), end(mSelection), inEntity);
		if (selectionIt != end(mSelection))
			mSelection.erase(selectionIt);
		else
			mSelection.push_back(inEntity);
		mLastInteractedEntity = inEntity;
	}

	b8 Scene::IsSelected(entt::entity inEntity)
	{
		return std::find(begin(mSelection), end(mSelection), inEntity) != end(mSelection);
	}

	void Scene::ExtendSelection(entt::entity inEntity)
	{
		if (mSelection.empty())
		{
			AddSelection(inEntity);
			return;
		}

		const entt::entity lastSelection = mLastInteractedEntity;
		if (lastSelection == inEntity)
		{
			RemoveSelection(inEntity);
			return;
		}
		
		const b8 cond = inEntity > lastSelection;
		mRegistry.sort<SelectionComponent>([cond](const entt::entity inA, const entt::entity inB)
		{
			return cond ? inA < inB : inA > inB;
		});

		auto selFn = IsSelected(inEntity) ? &Scene::RemoveSelection : &Scene::AddSelection;
		b8 hasStartedIterating = false;
		for (const entt::entity entity : mRegistry.view<SelectionComponent>())
		{
			if (entity == lastSelection)
			{
				hasStartedIterating = true;
			}

			if (entity == inEntity && hasStartedIterating)
			{
				(this->*selFn)(entity);
				break;
			}

			if (!hasStartedIterating)
				continue;

			(this->*selFn)(entity);
		}
	}
}