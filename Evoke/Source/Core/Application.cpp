#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Debug/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "InputIDs.h"
#include "Renderer/Buffer.h"
#include "Input.h"
#include "Renderer/EditorCameraController.h"
#include "Queue.h"
#include "Platform/Generic/Filesystem.h"
#include "Platform/Generic/Filewatcher.h"
#include "Renderer/ShaderLibrary.h"
#include "CommandLine.h"
#include "Platform/Generic/Dialog.h"
#include "Platform/RenderDoc/RenderDoc.h"
#include "Platform/RenderDoc/renderdoc_app.h"
#include "Renderer/Texture.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <type_traits>

#include "assimp/Importer.hpp"
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() :
		mMainWindow(Window::Create()),
		mContext(GraphicsContext::Create(mMainWindow->NativeWindow())),
		mAppRT(nullptr)
	{
		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe(EV_BIND(Application::OnWindowClose));
		mMainWindow->OnWindowResized.Subscribe(EV_BIND(Application::OnWindowResized));
		PushOverlay(new ImGuiLayer());
		mAppRT = RenderTarget2D::Create(1280, 720, { ETextureFormat::R8G8B8A8_UNORM, ETextureFormat::D24_UNORM_S8_UINT });
		mCameraController = MakeShared<EditorCameraController>();
	}

	Application::~Application()
	{
	}

	struct GlobalShaderData
	{
		glm::mat4 View;
		glm::mat4 Projection;
		glm::mat4 ViewProjection;
		f32 GameTime;
	};

	struct SkyboxCB
	{
		glm::mat4 SkyboxViewProjection;
	};

	struct Vertex
	{
		glm::vec3 Position;
		f32 PackedNormal;
		f32 PackedTangent;
		glm::vec2 UV0;
	};

	constexpr u32 NumVertexColors = 2;
	constexpr u32 NumUVSets = 6;

	struct TestVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		std::array<glm::vec4, NumVertexColors> Color;
		std::array<glm::vec2, NumUVSets> UV;
	};

	struct CubeVertex
	{
		glm::vec3 Position;
	};

	std::pair<std::vector<TestVertex>, std::vector<u32>> LoadMesh(StringView inPath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(inPath.data(), aiProcessPreset_TargetRealtime_MaxQuality);

		std::vector<TestVertex> vertices;
		std::vector<u32> indices;

		if (!scene)
		{
			EV_LOG(LogTemp, ELogLevel::Error, "{}", importer.GetErrorString());
			return std::make_pair(vertices, indices);
		}

		for (u32 i = 0; i < scene->mNumMeshes; i++)
		{
			auto* mesh = scene->mMeshes[i];

			vertices.reserve(vertices.size() + mesh->mNumVertices);
			indices.reserve(indices.size() + mesh->mNumFaces * 3u);

			for (u32 v = 0; v < mesh->mNumVertices; v++)
			{
				const auto& pos = mesh->mVertices[v];
				const auto& nrm = mesh->mNormals[v];
				const auto& tan = mesh->mTangents[v];
				const auto& bit = mesh->mBitangents[v];

				std::array<glm::vec4, NumVertexColors> colors{};
				for (u32 c = 0; c < NumVertexColors; c++)
				{
					if (c < mesh->GetNumColorChannels())
					{
						const auto& color = mesh->mColors[c][v];
						colors[c] = { color.r, color.g, color.b, color.a };
					}
				}

				std::array<glm::vec2, NumUVSets> uvs{};
				for (u32 c = 0; c < NumUVSets; c++)
				{
					if (c < mesh->GetNumUVChannels())
					{
						const auto& uv = mesh->mTextureCoords[c][v];
						uvs[c] = { uv.x, uv.y };
					}
				}

				vertices.push_back({
					glm::vec3{ pos.x, pos.y, pos.z },
					glm::vec3{ nrm.x, nrm.y, nrm.z },
					glm::vec3{ tan.x, tan.y, tan.z },
					glm::vec3{ bit.x, bit.y, bit.z },
					std::move(colors),
					std::move(uvs)
					});
			}

			for (u32 f = 0; f < mesh->mNumFaces; f++)
			{
				auto face = mesh->mFaces[f];
				for (u32 j = 0; j < 3; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}
		}

		importer.FreeScene();
		return std::make_pair(vertices, indices);
	}

	constexpr std::pair<std::array<CubeVertex, 8>, std::array<u32, 36>> MakeCube()
	{
		constexpr std::array<CubeVertex, 8> vertices
		{
			CubeVertex{ glm::vec3{-1.0f, -1.0f, -1.0f} },
			CubeVertex{ glm::vec3{1.0f, -1.0f, -1.0f} },
			CubeVertex{ glm::vec3{1.0f, 1.0f, -1.0f} },
			CubeVertex{ glm::vec3{-1.0f, 1.0f, -1.0f} },
			CubeVertex{ glm::vec3{-1.0f, 1.0f, 1.0f} },
			CubeVertex{ glm::vec3{1.0f, 1.0f, 1.0f} },
			CubeVertex{ glm::vec3{1.0f, -1.0f, 1.0f} },
			CubeVertex{ glm::vec3{-1.0f, -1.0f, 1.0f} },
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

	void Application::Run()
	{
		mContext->SetFaceCullingMethod(EFaceCulling::Back);

		auto fbxSelection = Dialog::OpenFile("Select mesh", ".", { "Mesh", "*.fbx;*.obj" });
		auto [vertices, indices] = LoadMesh(fbxSelection.value_or("Assets/TestSphere.fbx"));
		auto [vertices2, indices2] = LoadMesh("Assets/TestSphere.fbx");
		auto [cubeVertices, cubeIndices] = MakeCube();

		auto cubeVBO = VertexBuffer::Create(cubeVertices);
		auto cubeEBO = IndexBuffer::Create(cubeIndices);

		auto vbo = VertexBuffer::Create(vertices);
		auto ebo = IndexBuffer::Create(indices);

		auto vbo2 = VertexBuffer::Create(vertices2);
		auto ebo2 = IndexBuffer::Create(indices2);

		ShaderCompilerConfig cfg;
		cfg.AddDefine("NUM_VERTEX_COLORS", std::to_string(NumVertexColors));
		cfg.AddDefine("NUM_UV_SETS", std::to_string(NumUVSets));

		ShaderLibrary shaderLibrary;
		auto shader = shaderLibrary.Load("../Shaders/TestShader.hlsl", cfg);
		auto skyboxShader = shaderLibrary.Load("../Shaders/Skybox.hlsl");
		shader->Bind();

		auto anisotropicWrapSampler = Sampler::Create({ ETextureAddressMode::Wrap, ETextureFilter::Anisotropic, 16 });
		auto anisotropicClampSampler = Sampler::Create({ ETextureAddressMode::Clamp, ETextureFilter::Anisotropic, 16 });
		auto linearWrapSampler = Sampler::Create({ ETextureAddressMode::Mirror, ETextureFilter::Linear, 1 });

		auto testTexture1 = Texture2D::Create("Assets/TestTexture1.png");
		testTexture1->SetSampler(anisotropicWrapSampler);
		auto testTexture2 = Texture2D::Create("Assets/TestTexture2.png");
		testTexture2->SetSampler(anisotropicWrapSampler);

		TextureDescription skyDesc{ ETextureFormat::R32G32B32A32_Float };
		auto skyboxTexture = Texture2D::Create("Assets/kiara_1_dawn_2k.hdr", skyDesc);
		skyboxTexture->SetSampler(linearWrapSampler);

		auto shaderDataBuffer = ConstantBuffer::Create<GlobalShaderData>(0);
		auto skyboxCb = ConstantBuffer::Create<SkyboxCB>(1);

		const u32 i1Size = (u32)indices.size();
		const u32 i2Size = (u32)indices2.size();
		u32 numIndices = i2Size;

		auto vertexBuffer = vbo.get();

		// Capture a frame and open RenderDoc
		mMainWindow->OnKeyReleased.Subscribe([&](EKeyCode inKeyCode)
		{
			if (inKeyCode == EKeyCode::Enter)
			{
				RenderDoc::TriggerCapture();
				RenderDoc::LaunchUI();
			}

			if (inKeyCode == EKeyCode::M)
			{
				numIndices = i1Size;
				vertexBuffer = vbo.get();
			}
			else if (inKeyCode == EKeyCode::N)
			{
				numIndices = i2Size;
				vertexBuffer = vbo2.get();
			}
		});
		
		while (mIsRunning)
		{
			const f32 time = (f32)glfwGetTime();
			const f32 deltaTime = time - mLastFrameTime;
			mLastFrameTime = time;

			Update(deltaTime);

			mContext->BeginEvent("Scene");
			mAppRT->Bind();

			mContext->ClearColor();
			mContext->ClearDepth();
			mCameraController->Update(deltaTime);
			shaderLibrary.Update(deltaTime);

			auto& camera = mCameraController->Camera();
			shaderDataBuffer->View = camera.View();
			shaderDataBuffer->Projection = camera.Projection();
			shaderDataBuffer->ViewProjection = camera.ViewProjection();
			shaderDataBuffer->GameTime = time;
			shaderDataBuffer->Upload();

			skyboxCb->SkyboxViewProjection = camera.Projection() * glm::mat4(glm::mat3(shaderDataBuffer->View));
			skyboxCb->Upload();

			mContext->BeginEvent("Drawing something");
			mContext->SetFaceCullingMethod(EFaceCulling::Back);
			shader->Bind();
			testTexture1->Bind(0);
			testTexture2->Bind(1);
			vertexBuffer->Bind();
			mContext->DrawIndexed(numIndices);
			mContext->EndEvent();

			mContext->BeginEvent("Skybox");
			mContext->SetFaceCullingMethod(EFaceCulling::Front);
			skyboxShader->Bind();
			skyboxTexture->Bind(0);
			cubeVBO->Bind();
			mContext->DrawIndexed(cubeEBO->Size());
			mContext->EndEvent();

			mAppRT->Unbind();
			mContext->EndEvent();
		}
		
	}

	void Application::Update(f32 inDeltaTime)
	{
		for (Layer* layer : mLayerStack)
			layer->Update(inDeltaTime);

		mMainWindow->Update(inDeltaTime);

		mContext->BeginEvent("Frame Start");
		mContext->ClearColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		mContext->ClearDepth();
		mContext->EndEvent();
	}

	void Application::OnWindowClose()
	{
		Close();
	}

	void Application::OnWindowResized(u32 inWidth, u32 inHeight)
	{
		//mContext->SetViewport(0, 0, inWidth, inHeight);
		//Update(0.0f);
	}

	void Application::PushLayer(Layer* inLayer)
	{
		mLayerStack.PushLayer(inLayer);
		inLayer->Attached();
	}

	void Application::PushOverlay(Layer* inLayer)
	{
		mLayerStack.PushOverlay(inLayer);
		inLayer->Attached();
	}

}