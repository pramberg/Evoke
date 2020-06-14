#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Debug/ImGuiLayer.h"
#include "glad/glad.h"
#include <fstream>

bool loadBinaryShader(const char* fileName, GLuint stage, GLuint binaryFormat, GLuint& shader)
{
	std::ifstream shaderFile;
	shaderFile.open(fileName, std::ios::binary | std::ios::ate);
	if (shaderFile.is_open())
	{
		size_t size = shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);
		char* bin = new char[size];
		shaderFile.read(bin, size);

		GLint status;
		shader = glCreateShader(stage);                                 // Create a new shader
		glShaderBinary(1, &shader, binaryFormat, bin, size);            // Load the binary shader file
		glSpecializeShaderARB(shader, "main", 0, nullptr, nullptr);     // Set main entry point (required, no specialization used in this example)
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);              // Check compilation status
		return status;
	}
	else
	{
		std::cerr << "Could not open \"" << fileName << "\"" << std::endl;
		return false;
	}
}

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

	Application::~Application()
	{
	}

	void Application::Run()
	{
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) { EV_INFO("{}", message); }, nullptr);
		u32 vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		f32 vertices[]{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		u32 vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		const c8* vertexShaderSource = "#version 430 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		u32 vertexShader;// = glCreateShader(GL_VERTEX_SHADER);
		/*glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);*/

		i32 success;
		c8 infoLog[512];
		/*glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			EV_CORE_ERROR("Vertex shader failed to compile: {}", infoLog);
		}*/

		/*const c8* fragmentShaderSource = "#version 430 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0";

		u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			EV_CORE_ERROR("Fragment shader failed to compile: {}", infoLog);
		}*/
		u32 fragmentShader;
		b8 loadSuccessful = loadBinaryShader("../Shaders/vert.spirv", GL_VERTEX_SHADER, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, vertexShader);
		loadSuccessful &= loadBinaryShader("../Shaders/frag.spirv", GL_FRAGMENT_SHADER, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, fragmentShader);
		EV_CORE_ASSERT(loadSuccessful, "Failed to load SPIR-V binary");
		u32 shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			EV_CORE_ERROR("Program linking failed: {}", infoLog);
		}

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

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

		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
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