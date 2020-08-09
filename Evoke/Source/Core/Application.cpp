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

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <type_traits>

#include "assimp/Importer.hpp"
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Platform/RenderDoc/RenderDoc.h"
#include "Platform/RenderDoc/renderdoc_app.h"

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() : mMainWindow(Window::Create()), mContext(GraphicsContext::Create(mMainWindow->NativeWindow()))
	{
		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe(EV_BIND(Application::OnWindowClose));
		mMainWindow->OnWindowResized.Subscribe(EV_BIND(Application::OnWindowResized));
		PushOverlay(new ImGuiLayer());
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
			indices.reserve(indices.size() + mesh->mNumFaces * 3L);

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

	void Application::Run()
	{
		mContext->SetFaceCullingMethod(EFaceCulling::Back);

		auto fbxSelection = Dialog::OpenFile("Select mesh", ".", { "Mesh", "*.fbx;*.obj" });
		auto [vertices, indices] = LoadMesh(fbxSelection.value_or("Assets/TestSphere.fbx"));
		auto [vertices2, indices2] = LoadMesh("Assets/TestSphere.fbx");

		auto vbo = VertexBuffer::Create(vertices);
		auto ebo = Buffer::Create<EBufferType::Index>(indices);

		auto vbo2 = VertexBuffer::Create(vertices2);
		auto ebo2 = Buffer::Create<EBufferType::Index>(indices2);

		ShaderCompilerConfig cfg = ShaderCompilerConfig::BasicConfig();
		cfg.AddDefine("NUM_VERTEX_COLORS", std::to_string(NumVertexColors));
		cfg.AddDefine("NUM_UV_SETS", std::to_string(NumUVSets));

		ShaderLibrary shaderLibrary;
		auto shader = shaderLibrary.Load("../Shaders/TestShader.hlsl", cfg);

		EditorCameraController cameraController;
		auto shaderDataBuffer = ConstantBuffer::Create<GlobalShaderData>(0);

		const size_t i1Size = indices.size();
		const size_t i2Size = indices2.size();
		size_t numIndices = i2Size;

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
				vbo->Bind();
			}
			else if (inKeyCode == EKeyCode::N)
			{
				numIndices = i2Size;
				vbo2->Bind();
			}
		});

		while (mIsRunning)
		{
			const f32 time = (f32)glfwGetTime();
			const f32 deltaTime = time - mLastFrameTime;
			mLastFrameTime = time;

			Update(deltaTime);
			cameraController.Update(deltaTime);
			shaderLibrary.Update(deltaTime);

			auto& camera = cameraController.Camera();
			shaderDataBuffer->View = camera.View();
			shaderDataBuffer->Projection = camera.Projection();
			shaderDataBuffer->ViewProjection = camera.ViewProjection();
			shaderDataBuffer->GameTime = time;
			shaderDataBuffer->Upload();

			shader->Bind();

			mContext->BeginEvent("Drawing something");
			glDrawElements(GL_TRIANGLES, (i32)numIndices, GL_UNSIGNED_INT, nullptr);
			mContext->EndEvent();
		}
	}

	void Application::Update(f32 inDeltaTime)
	{
		for (Layer* layer : mLayerStack)
			layer->Update(inDeltaTime);

		mMainWindow->Update(inDeltaTime);

		mContext->BeginEvent("Frame Start");
		mContext->ClearColor({ 0.1f, 0.0f, 0.1f, 1.0f });
		mContext->ClearDepth();
		mContext->EndEvent();
	}

	void Application::OnWindowClose()
	{
		Close();
	}

	void Application::OnWindowResized(u32 inWidth, u32 inHeight)
	{
		mContext->SetViewport(0, 0, inWidth, inHeight);
		Update(0.0f);
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