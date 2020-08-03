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

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <type_traits>
#include "Renderer/ShaderLibrary.h"

#include "assimp/Importer.hpp"
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() : mMainWindow(Window::Create()), mContext(GraphicsContext::Create(mMainWindow->NativeWindow()))
	{
		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe(EV_BIND_0(Application::OnWindowClose));
		mMainWindow->OnWindowResized.Subscribe(EV_BIND_2(Application::OnWindowResized));
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

	struct TestVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	void Application::Run()
	{
		mContext->SetFaceCullingMethod(EFaceCulling::Back);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			"W:/Projects/Evoke/DefaultProject/Assets/TestSphere.fbx",
			aiProcessPreset_TargetRealtime_MaxQuality 
		);

		std::vector<TestVertex> vertices;
		std::vector<u32> indices;

		if (scene)
		{
			for (u32 i = 0; i < scene->mNumMeshes; i++)
			{
				auto* mesh = scene->mMeshes[i];

				for (u32 v = 0; v < mesh->mNumVertices; v++)
				{
					auto vert = mesh->mVertices[v];
					auto nrm = mesh->mNormals[v];
					vertices.push_back({ { vert.x, vert.y, vert.z }, { nrm.x, nrm.y, nrm.z, 1.0f } });
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
		}
		else
		{
			EV_LOG(LogTemp, ELogLevel::Error, "{}", importer.GetErrorString());
			vertices.push_back({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } });
			vertices.push_back({ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } });
			vertices.push_back({ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } });
			vertices.push_back({ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } });
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(3);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(3);
		}
		importer.FreeScene();

		u32 vao;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		u32 vbo;
		glCreateBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TestVertex), vertices.data(), GL_STATIC_DRAW);

		std::vector<i32> sizeStuff;
		sizeStuff.push_back(3);
		sizeStuff.push_back(4);

		i32 sumStride = 0;
		for (i32 i = 0; i < sizeStuff.size(); i++)
		{
			glVertexAttribFormat(i, sizeStuff[i], GL_FLOAT, GL_FALSE, sumStride);
			glVertexAttribBinding(i, 0);
			glEnableVertexAttribArray(i);
			sumStride += sizeStuff[i] * sizeof(f32);
		}

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), nullptr);

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(3 * sizeof(f32)));

		u32 ebo;
		glCreateBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

		ShaderLibrary shaderLibrary;
		auto shader = shaderLibrary.Load("../Shaders/TestShader.hlsl");
		shader->Bind();
		
		glm::vec3 test{ 10, 20, 40 };
		auto x = reinterpret_cast<f32*>(&test);
		EV_LOG(LogTemp, ELogLevel::Info, "{}", *(x + 1));
		EV_LOG(LogTemp, ELogLevel::Info, "{}", Filesystem::MatchPattern("C:/TestDir/TestFile.txt", "*.txt"));

		EditorCameraController cameraController;
		auto shaderDataBuffer = ConstantBuffer::Create<GlobalShaderData>(0);

		while (mIsRunning)
		{
			const f32 time = (f32)glfwGetTime();
			const f32 deltaTime = time - mLastFrameTime;
			mLastFrameTime = time;

			Update(deltaTime);
			cameraController.Update(deltaTime);
			shaderLibrary.Update(deltaTime);

			shaderDataBuffer->View = cameraController.Camera().View();
			shaderDataBuffer->Projection = cameraController.Camera().Projection();
			shaderDataBuffer->ViewProjection = cameraController.Camera().ViewProjection();
			shaderDataBuffer->GameTime = time;
			shaderDataBuffer->Upload();

			shader->Bind();

			mContext->BeginEvent("Drawing something");
			glBindVertexArray(vao);
			glBindVertexBuffer(0, vbo, 0, sizeof(TestVertex));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			mContext->EndEvent();
		}
	}

	void Application::Update(f32 inDeltaTime)
	{
		for (Layer* layer : mLayerStack)
			layer->Update(inDeltaTime);

		mMainWindow->Update(inDeltaTime);

		mContext->ClearColor({ 0.1f, 0.0f, 0.1f, 1.0f });
		mContext->ClearDepth();
	}

	void Application::OnWindowClose()
	{
		Close();
	}

	void Application::OnWindowResized(u32 inWidth, u32 inHeight)
	{
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