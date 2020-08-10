#pragma once
#include "Core/Core.h"

namespace Evoke
{
	enum class EFaceCulling : c8
	{
		Front,
		Back,
		FrontAndBack,
		Disabled
	};

	enum class EPrimitiveType : c8
	{
		Point,
		Line,
		LineStrip,
		Triangle,
		TriangleStrip,
		LineAdj,
		LineStripAdj,
		TriangleAdj,
		TriangleStripAdj,
		Patch1,
		Patch2,
		Patch3
	};

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
	
		virtual void SwapBuffers() = 0;
		virtual void SetViewport(u32 inX, u32 inY, u32 inWidth, u32 inHeight) = 0;
		virtual void ClearColor(const glm::vec4 inColor = { 0.0f, 0.0f, 0.0f, 1.0f }) = 0;
		virtual void ClearDepth(f32 inDepth = 1.0f) = 0;
		virtual void ClearStencil(u32 inStencil = 0) = 0;
		
		virtual void DrawIndexed(u32 inNumIndices) = 0;
		virtual void SetFaceCullingMethod(EFaceCulling inMethod) = 0;

		virtual void BeginEvent(StringView inEventName) = 0;
		virtual void EndEvent() = 0;

	public:
		static TUniquePtr<GraphicsContext> Create(void* inWindow);
	};
}