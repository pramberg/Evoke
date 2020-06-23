#pragma once
#include "Core/Core.h"
#include "Renderer.h"

namespace Evoke
{
	template<typename T>
	class TConstantBuffer
	{
	public:
		TConstantBuffer() : mData() {}
		TConstantBuffer(const T& inData) : mData(inData) {}
		T& Data() { return mData; }

	public:
		virtual ~TConstantBuffer() = default;
		virtual void Update() = 0;
		virtual u32 GetSlot() = 0;
		virtual void SetSlot(u32 inSlot) = 0;

	protected:
		T mData;
	};
}

// Needs to be included here since they inherit from TUniformBuffer.
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Evoke
{
	class ConstantBuffer
	{
	public:
		template<typename T>
		static std::shared_ptr<TConstantBuffer<T>> Create(const T& inData, u32 inSlot)
		{
			switch (Renderer::GetAPI())
			{
			case ERenderAPI::OpenGL: return std::make_shared<TOpenGLConstantBuffer<T>>(inData, inSlot);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}

		template<typename T>
		static std::shared_ptr<TConstantBuffer<T>> Create(u32 inSlot)
		{
			switch (Renderer::GetAPI())
			{
			case ERenderAPI::OpenGL: return std::make_shared<TOpenGLConstantBuffer<T>>(inSlot);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}
	};

}