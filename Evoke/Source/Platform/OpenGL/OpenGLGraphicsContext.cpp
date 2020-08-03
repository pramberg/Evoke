#include "PCH.h"
#include "OpenGLGraphicsContext.h"

#include "glad\glad.h"

namespace Evoke
{

	void OpenGLMessageCallback(
		GLenum inSource,
		GLenum inType,
		GLuint inId,
		GLenum inSeverity,
		GLsizei inLength,
		const GLchar* inMessage,
		const void* inUserPtr)
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
			//break;
		case GL_DEBUG_TYPE_POP_GROUP:
			type = "Pull group";
			return;
			//break;
		}

		EV_LOG(LogRHI, logLevel, "\nGL Debug:\n  Source: {}\n  Type: {}\n  Id: {}  \n  Message: {}", source, type, inId, inMessage);
	}

	OpenGLGraphicsContext::OpenGLGraphicsContext(void* inWindow) : mWindowHandle(static_cast<GLFWwindow*>(inWindow))
	{
		glfwMakeContextCurrent(mWindowHandle);
		i32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EV_CORE_ASSERT(status, "Failed to initialize Glad.");

		EV_LOG(LogRHI, ELogLevel::Info,
			"\nOpenGL graphics context:\n  Vendor: {}\n  Hardware: {}\n  Version: {}", 
			glGetString(GL_VENDOR),
			glGetString(GL_RENDERER),
			glGetString(GL_VERSION)
		);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SetFaceCullingMethod(EFaceCulling::Back);

#ifdef EV_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
	}

	OpenGLGraphicsContext::~OpenGLGraphicsContext()
	{
		
	}
	
	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}
	
	void OpenGLGraphicsContext::SetViewport(u32 inX, u32 inY, u32 inWidth, u32 inHeight)
	{
		glViewport(inX, inY, inWidth, inHeight);
	}
	
	void OpenGLGraphicsContext::ClearColor(const glm::vec4 inColor)
	{
		glClearColor(inColor.r, inColor.g, inColor.b, inColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void OpenGLGraphicsContext::ClearDepth(f32 inDepth)
	{
		glClearDepth(inDepth);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLGraphicsContext::ClearStencil(u32 inStencil)
	{
		glStencilMask(inStencil);
		glClear(GL_STENCIL_BUFFER_BIT);
	}
	
	void OpenGLGraphicsContext::DrawIndexed()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void OpenGLGraphicsContext::BeginEvent(StringView inEventName)
	{
		glPushDebugGroup(GL_DEBUG_SOURCE_THIRD_PARTY, 0, -1, inEventName.data());
	}

	void OpenGLGraphicsContext::EndEvent()
	{
		glPopDebugGroup();
	}

	void OpenGLGraphicsContext::SetFaceCullingMethod(EFaceCulling inMethod)
	{
		glEnable(GL_CULL_FACE);
		switch (inMethod)
		{
		case Evoke::EFaceCulling::Front:
			glCullFace(GL_FRONT);
			break;
		case Evoke::EFaceCulling::Back:
			glCullFace(GL_BACK);
			break;
		case Evoke::EFaceCulling::FrontAndBack:
			glCullFace(GL_FRONT_AND_BACK);
			break;
		case EFaceCulling::Disabled:
			glDisable(GL_CULL_FACE);
			break;
		default:
			glCullFace(GL_BACK);
			break;
		}
	}
}
