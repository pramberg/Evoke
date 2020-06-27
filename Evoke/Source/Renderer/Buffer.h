#pragma once
#include "Core/Core.h"
#include "Renderer.h"

namespace Evoke
{
	template<typename T>
	class TBuffer
	{
	public:
		TBuffer() : mData() {}
		TBuffer(const T& inData) : mData(inData) {}
		T& Data() { return mData; }

	protected:
		T mData;
	};

	template<typename T>
	class TConstantBuffer : public TBuffer<T>
	{
	public:
		virtual ~TConstantBuffer() = default;
		virtual void Update() = 0;
		virtual u32 GetSlot() = 0;
		virtual void SetSlot(u32 inSlot) = 0;
	};
}

// Needs to be included here since they inherit from TConstantBuffer.
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Evoke
{
	class ConstantBuffer
	{
	public:
		template<typename T>
		static TUniquePtr<TConstantBuffer<T>> Create(const T& inData, u32 inSlot)
		{
			switch (Renderer::GetAPI())
			{
			case ERenderAPI::OpenGL: return MakeUnique<TOpenGLConstantBuffer<T>>(inData, inSlot);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}

		template<typename T>
		static TUniquePtr<TConstantBuffer<T>> Create(u32 inSlot)
		{
			switch (Renderer::GetAPI())
			{
			case ERenderAPI::OpenGL: return MakeUnique<TOpenGLConstantBuffer<T>>(inSlot);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}
	};

}