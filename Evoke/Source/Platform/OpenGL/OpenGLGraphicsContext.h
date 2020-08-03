#pragma once
#include "Core/Core.h"
#include "Renderer/GraphicsContext.h"

#include "GLFW/glfw3.h"

namespace Evoke
{
	class OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext(void* inWindow);
		virtual ~OpenGLGraphicsContext() override;
		virtual void SwapBuffers() override;
		virtual void SetViewport(u32 inX, u32 inY, u32 inWidth, u32 inHeight) override;
		virtual void ClearColor(const glm::vec4 inColor) override;
		virtual void ClearDepth(f32 inDepth) override;
		virtual void ClearStencil(u32 inStencil) override;

		virtual void DrawIndexed() override;
		virtual void SetFaceCullingMethod(EFaceCulling inMethod) override;

		virtual void BeginEvent(StringView inEventName) override;
		virtual void EndEvent() override;

	private:
		GLFWwindow* mWindowHandle;
	};
}