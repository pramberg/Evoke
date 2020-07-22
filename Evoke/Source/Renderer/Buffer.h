#pragma once
#include "Core/Core.h"
#include "Renderer.h"

namespace Evoke
{
	template<typename T>
	class TBuffer : public T
	{
	public:
		TBuffer() {}
		TBuffer(const T& inData) : T(inData) {}
		T& Data() { return *this; }
	};

	template<typename T>
	class TConstantBuffer : public TBuffer<T>
	{
	public:
		virtual ~TConstantBuffer() = default;
		virtual void Upload() = 0;
		virtual u32 Slot() = 0;
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
			static_assert(std::is_trivial<T>::value, "Constant buffer type should be trivial");
			switch (Renderer::API())
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
			static_assert(std::is_trivial<T>::value, "Constant buffer type should be trivial");
			switch (Renderer::API())
			{
			case ERenderAPI::OpenGL: return MakeUnique<TOpenGLConstantBuffer<T>>(inSlot);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}
	};

}