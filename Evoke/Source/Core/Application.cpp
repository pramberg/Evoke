#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Debug/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "InputIDs.h"

#include "glad/glad.h"

namespace Evoke
{
	Application* Application::sApplication = nullptr;

	Application::Application() : mMainWindow(std::unique_ptr<Window>(Window::Create()))
	{
		EV_CORE_ASSERT(!sApplication, "Application already exists");
		sApplication = this;

		mMainWindow->OnWindowClosed.Subscribe(EV_BIND_0(Application::OnWindowClose));
		mMainWindow->OnWindowResized.Subscribe(EV_BIND_2(Application::OnWindowResized));
		PushOverlay(new ImGuiLayer());
	}

	struct MeshBufferLayout
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

	Application::~Application()
	{
	}

	static std::shared_ptr<Shader> gShader;

	struct TestBuffer
	{
		glm::vec4 Something;
	};

	struct TestBuffer2
	{
		glm::vec4 SomethingElse;
		glm::vec4 SomethingElseAgain;
		float TestArray[4];
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

		gShader = Shader::Create("../Shaders/TestShader.hlsl");
		gShader->Bind();

		mMainWindow->OnKeyPressed.Subscribe([](i32 inKeycode, i32 inRepeatCount)
		{
			if (inKeycode == EV_KEY_ENTER)
			{
				gShader->Recompile();
				gShader->Bind();
			}
		});

		// Remember: This needs to be rebound on shader recompile
		i32 program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);

		TestBuffer testBufferData{ glm::vec4(1.0f, 0.0f, 0.0f, 0) };
		u32 testBuffer;
		glCreateBuffers(1, &testBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, testBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(TestBuffer), &testBufferData, GL_STATIC_DRAW);
		glUniformBlockBinding(program, 0, 0);

		TestBuffer2 testBufferData2{ glm::vec4(0.1f, 0.0f, 1.0f, 0), glm::vec4(0.2f, 1.0f, 0.4f, 0), { 1.0f, 0.5f, 0.5f, 1.0f } };
		u32 testBuffer2;
		glCreateBuffers(1, &testBuffer2);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, testBuffer2);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(TestBuffer2), &testBufferData2, GL_STATIC_DRAW);
		glUniformBlockBinding(program, 0, 1);

		while (mIsRunning)
		{
			Update();
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