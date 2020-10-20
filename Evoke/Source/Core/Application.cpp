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

#include "Platform/Generic/Paths.h"
#include "Renderer/Material.h"
#include "Renderer/StaticMesh.h"
#include "imgui.h"

#include "Debug/Widgets/Outliner.h"
#include "ECS/Components.h"
#include "Assets\AssetRegistry.h"
#include "Renderer/RenderTarget.h"
#include "ECS/Scene.h"

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() :
		mMainWindow(Window::Create()),
		mContext(GraphicsContext::Create(mMainWindow->NativeWindow())),
		mAppRT(nullptr),
		mActiveScene(nullptr)
	{
		if (CommandLine::HasParameter("-Project"))
		{
			Paths::SetProjectPath(CommandLine::Argument("-Project").value());
		}
		else
		{
			auto projectFolder = Dialog::SelectFolder("Select Project", ".");
			Paths::SetProjectPath(projectFolder.value());
		}

		mAssetRegistry = MakeUnique<AssetRegistry>();

		EV_LOG(LogTemp, ELogLevel::Info, "Project: {}", Paths::ProjectPath());

		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe(EV_BIND(Application::OnWindowClose));
		mMainWindow->OnWindowResized.Subscribe(EV_BIND(Application::OnWindowResized));
		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);

		mAppRT = RenderTarget2D::Create(1920, 1080, { ETextureFormat::R8G8B8A8_UNORM, ETextureFormat::D24_UNORM_S8_UINT });
		mCameraController = MakeShared<EditorCameraController>();
		mActiveScene = MakeUnique<Scene>();
	}

	Application::~Application()
	{
	}

	struct GlobalShaderData
	{
		glm::mat4 View;
		glm::mat4 Projection;
		glm::mat4 ViewProjection;
		glm::vec3 CameraPosition;
		f32 GameTime;
	};

	void Application::Run()
	{
		mContext->SetFaceCullingMethod(EFaceCulling::Back);

		auto shaderDataBuffer = ConstantBuffer::Create<GlobalShaderData>(0);

		// Capture a frame and open RenderDoc
		mMainWindow->OnKeyReleased.Subscribe([&](EKeyCode inKeyCode)
		{
			if (inKeyCode == EKeyCode::Home)
			{
				RenderDoc::TriggerCapture();
				RenderDoc::LaunchUI();
			}
		});

		auto entity = mActiveScene->CreateEntity();
		entity.AddComponent<StaticMeshComponent>("Assets\\TestAllAttributes.fbx");

		auto entity2 = mActiveScene->CreateEntity();
		entity2.AddComponent<StaticMeshComponent>("Assets\\TestSphere.fbx");
		auto& tf = entity2.GetComponent<TransformComponent>();
		tf.Location = glm::vec3{ 4, 0, -7 };

		while (mIsRunning)
		{
			const f32 time = (f32)glfwGetTime();
			const f32 deltaTime = time - mLastFrameTime;
			mLastFrameTime = time;

			mContext->BeginEvent("Scene");
			mCameraController->Update(deltaTime);

			auto& camera = mCameraController->Camera();
			shaderDataBuffer->View = camera.View();
			shaderDataBuffer->Projection = camera.Projection();
			shaderDataBuffer->ViewProjection = camera.ViewProjection();
			shaderDataBuffer->CameraPosition = glm::vec3(glm::inverse(camera.View()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			shaderDataBuffer->GameTime = time;
			shaderDataBuffer->Upload();

			Update(deltaTime);

			mContext->EndEvent();
		}
	}

	void Application::Update(f32 inDeltaTime)
	{
		RenderScene(inDeltaTime);

		for (Layer* layer : mLayerStack)
		{
			if (layer->Name() == "ImGuiLayer")
				mAppRT->Unbind();
			layer->Update(inDeltaTime, mContext.get());

		}

		mMainWindow->Update(inDeltaTime);
		Material::gShaderLibrary->Update(inDeltaTime);
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

	void Application::RenderScene(f32 inDeltaTime)
	{
		mContext->BeginEvent("Frame Start");
		mAppRT->Bind();
		mContext->ClearColor();
		mContext->ClearDepth();
		mContext->EndEvent();

		mActiveScene->Update(inDeltaTime, mContext.get());
		mAppRT->Unbind();
	}

}