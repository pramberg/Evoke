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

#include "Renderer/ShaderLibrary.h"

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() : mMainWindow(Window::Create())
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

	struct TestVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	void Application::Run()
	{
		//glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum inSource, GLenum inType, GLuint inId, GLenum inSeverity, GLsizei inLength, const GLchar* inMessage, const void* inUserPtr)
		{
			auto logLevel = EV_INFO;
			switch (inSeverity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				logLevel = EV_ERROR;
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				logLevel = EV_WARNING;
				break;
			case GL_DEBUG_SEVERITY_LOW:
				logLevel = EV_INFO;
				break;
			}

			string source;
			switch (inSource)
			{
			case GL_DEBUG_SOURCE_API:
				source = "API";
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				source = "Window system";
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				source = "Shader compiler";
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				source = "Third party";
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				source = "Application";
				break;
			case GL_DEBUG_SOURCE_OTHER:
				source = "Other";
				break;
			}

			string type;
			switch (inType)
			{
			case GL_DEBUG_TYPE_ERROR:
				type = "Error";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				type = "Deprecated behavior";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				type = "Undefined behavior";
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				type = "Portability";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				type = "Performance";
				break;
			case GL_DEBUG_TYPE_OTHER:
				type = "Other";
				break;
			case GL_DEBUG_TYPE_MARKER:
				type = "Marker";
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				type = "Push group";
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				type = "Pull group";
				break;
			}

			EV_LOG(LogRHI, logLevel, "\nGL Debug:\n  Source: {}\n  Type: {}\n  Id: {}  \n  Message: {}", source, type, inId, inMessage);
		}, nullptr);
		//glEnable(GL_CULL_FACE); // cull face
		//glCullFace(GL_BACK); // cull back face
		//glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

		TestVertex vertices[]
		{
			{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
			{ { 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
			{ { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
			{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
		};
		
		u32 indices[]
		{
			0, 1, 3,
			1, 2, 3
		};

		std::vector<i32> sizeStuff;
		sizeStuff.push_back(3);
		sizeStuff.push_back(4);

		u32 vao;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		u32 vbo;
		glCreateBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		for (i32 i = 0; i < sizeStuff.size(); i++)
		{
			glEnableVertexAttribArray(i);
			i32 stride = i > 0 ? sizeStuff[i - 1] : 0;
			glVertexAttribPointer(i, sizeStuff[i], GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(stride * sizeof(f32)));
		}

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), nullptr);

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(3 * sizeof(f32)));

		u32 ebo;
		glCreateBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		auto shaderLibrary = ShaderLibrary({ "../Shaders/" });
		auto shader = shaderLibrary.Load("../Shaders/TestShader.hlsl");
		shader->Bind();
		
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

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	void Application::Update(f32 inDeltaTime)
	{
		for (Layer* layer : mLayerStack)
			layer->Update(inDeltaTime);

		mMainWindow->Update(inDeltaTime);

		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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