#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Debug/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "InputIDs.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/Buffer.h"
#include "Input.h"

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

	static TSharedPtr<Shader> gShader;

	struct GlobalShaderData
	{
		glm::mat4 View;
		glm::mat4 Projection;
		glm::mat4 ViewProjection;
		f32 GameTime;
	};

	void Application::Run()
	{
		glEnable(GL_DEBUG_OUTPUT);
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
		
		u32 vao;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		f32 vertices[]{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		
		u32 vbo;
		glCreateBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		auto shaderDataBuffer = ConstantBuffer::Create<GlobalShaderData>(0);

		auto& gsData = shaderDataBuffer->Data();
		gsData.Projection = glm::perspectiveFov(glm::radians(70.0f), 1280.0f, 720.0f, 0.01f, 1000.0f);

		gShader = Shader::Create("../Shaders/TestShader.hlsl");
		gShader->Bind();

		mMainWindow->OnKeyPressed.Subscribe([](EKeyCode inKeycode, i32 inRepeatCount)
		{
			if (inKeycode == EKeyCode::Enter)
			{
				gShader->Recompile();
				gShader->Bind();
			}
		});

		while (mIsRunning)
		{
			Update();
			static glm::vec3 gPosition(0, 0, 1);
			static glm::vec3 gRotation(0, 0, 0);
			
			if (Input::IsKeyPressed(EKeyCode::W))
				gPosition.z -= 0.1f;

			if (Input::IsKeyPressed(EKeyCode::S))
				gPosition.z += 0.1f;

			if (Input::IsKeyPressed(EKeyCode::A))
				gPosition.x -= 0.1f;

			if (Input::IsKeyPressed(EKeyCode::D))
				gPosition.x += 0.1f;

			if (Input::IsKeyPressed(EKeyCode::E))
				gPosition.y += 0.1f;

			if (Input::IsKeyPressed(EKeyCode::Q))
				gPosition.y -= 0.1f;


			if (Input::IsKeyPressed(EKeyCode::Left))
				gRotation.y -= 0.01;

			if (Input::IsKeyPressed(EKeyCode::Right))
				gRotation.y += 0.01;

			if (Input::IsKeyPressed(EKeyCode::Up))
				gRotation.x -= 0.01;

			if (Input::IsKeyPressed(EKeyCode::Down))
				gRotation.x += 0.01;

			auto rot = glm::rotate(glm::identity<glm::mat4>(), gRotation.x, glm::vec3(1, 0, 0)) * glm::rotate(glm::identity<glm::mat4>(), gRotation.y, glm::vec3(0, 1, 0));

			gsData.View = glm::translate(glm::mat4(1.0f), -gPosition) * rot;
			gsData.ViewProjection = gsData.Projection * gsData.View;
			gsData.GameTime = (f32)glfwGetTime();
			shaderDataBuffer->Update();

			gShader->Bind();

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	void Application::Update()
	{
		for (Layer* layer : mLayerStack)
			layer->Update();

		mMainWindow->Update();

		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Application::OnWindowClose()
	{
		Close();
	}

	void Application::OnWindowResized(u32 inWidth, u32 inHeight)
	{
		Update();
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