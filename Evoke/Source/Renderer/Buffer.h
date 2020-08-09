#pragma once
#include "Core/Core.h"
#include "Renderer.h"

namespace Evoke
{
	enum class EBufferType : c8
	{
		Vertex,
		Index,
		Constant,
	};

	enum class ECPUAccess : c8
	{
		Read,
		Write,
		ReadWrite,
		None,
	};

	enum class EBufferUsage : c8
	{
		Invalid = 0,
		Static,
		Dynamic,
		Immutable,
		Stream,
	};

	enum class EShaderDataType : c8
	{
		Invalid = 0,
		Float, Float2, Float3, Float4,
		Float2x2, Float2x3, Float3x2,
		Float3x3, Float3x4, Float4x3,
		Float4x4, Float4x2, Float2x4,
		Int, Int2, Int3, Int4,
		Bool, Bool2, Bool3, Bool4,
	};

	static u32 ShaderDataTypeSize(EShaderDataType inType)
	{
		switch (inType)
		{
		case Evoke::EShaderDataType::Float:    return 4;
		case Evoke::EShaderDataType::Float2:   return 4 * 2;
		case Evoke::EShaderDataType::Float3:   return 4 * 3;
		case Evoke::EShaderDataType::Float4:   return 4 * 4;
		case Evoke::EShaderDataType::Float2x2: return 4 * 2 * 2;
		case Evoke::EShaderDataType::Float2x3: return 4 * 2 * 3;
		case Evoke::EShaderDataType::Float3x2: return 4 * 3 * 2;
		case Evoke::EShaderDataType::Float3x3: return 4 * 3 * 3;
		case Evoke::EShaderDataType::Float3x4: return 4 * 3 * 4;
		case Evoke::EShaderDataType::Float4x3: return 4 * 4 * 3;
		case Evoke::EShaderDataType::Float4x4: return 4 * 4 * 4;
		case Evoke::EShaderDataType::Float4x2: return 4 * 4 * 2;
		case Evoke::EShaderDataType::Float2x4: return 4 * 2 * 4;
		case Evoke::EShaderDataType::Int:      return 4;
		case Evoke::EShaderDataType::Int2:     return 4 * 2;
		case Evoke::EShaderDataType::Int3:     return 4 * 3;
		case Evoke::EShaderDataType::Int4:     return 4 * 4;
		case Evoke::EShaderDataType::Bool:     return 1;
		case Evoke::EShaderDataType::Bool2:    return 1 * 2;
		case Evoke::EShaderDataType::Bool3:    return 1 * 3;
		case Evoke::EShaderDataType::Bool4:    return 1 * 4;
		}
		EV_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	static u32 ShaderDataTypeComponentCount(EShaderDataType inType)
	{
		switch (inType)
		{
		case Evoke::EShaderDataType::Float:    return 1;
		case Evoke::EShaderDataType::Float2:   return 2;
		case Evoke::EShaderDataType::Float3:   return 3;
		case Evoke::EShaderDataType::Float4:   return 4;
		case Evoke::EShaderDataType::Float2x2: return 2;
		case Evoke::EShaderDataType::Float2x3: return 2;
		case Evoke::EShaderDataType::Float3x2: return 3;
		case Evoke::EShaderDataType::Float3x3: return 3;
		case Evoke::EShaderDataType::Float3x4: return 3;
		case Evoke::EShaderDataType::Float4x3: return 4;
		case Evoke::EShaderDataType::Float4x4: return 4;
		case Evoke::EShaderDataType::Float4x2: return 4;
		case Evoke::EShaderDataType::Float2x4: return 2;
		case Evoke::EShaderDataType::Int:      return 1;
		case Evoke::EShaderDataType::Int2:     return 2;
		case Evoke::EShaderDataType::Int3:     return 3;
		case Evoke::EShaderDataType::Int4:     return 4;
		case Evoke::EShaderDataType::Bool:     return 1;
		case Evoke::EShaderDataType::Bool2:    return 2;
		case Evoke::EShaderDataType::Bool3:    return 3;
		case Evoke::EShaderDataType::Bool4:    return 4;
		}
		EV_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	static constexpr i32 ShaderDataTypeNumVectors(EShaderDataType inType)
	{
		switch (inType)
		{
		case Evoke::EShaderDataType::Float2x2: return 2;
		case Evoke::EShaderDataType::Float2x3: return 3;
		case Evoke::EShaderDataType::Float3x2: return 2;
		case Evoke::EShaderDataType::Float3x3: return 3;
		case Evoke::EShaderDataType::Float3x4: return 4;
		case Evoke::EShaderDataType::Float4x3: return 3;
		case Evoke::EShaderDataType::Float4x4: return 4;
		case Evoke::EShaderDataType::Float4x2: return 2;
		case Evoke::EShaderDataType::Float2x4: return 4;
		default: return 1;
		}
	}

	template<typename TDataType, EBufferType TBufferType>
	class TBuffer
	{
	public:
		TBuffer() = default;

		virtual void Bind() = 0;
		virtual void SetData(const std::vector<TDataType>& inData) = 0;
	};

	template<typename TDataType>
	class TVertexBuffer
	{
	public:
		TVertexBuffer() = default;
		virtual ~TVertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void SetData(const std::vector<TDataType>& inData) = 0;
	};

	template<typename T>
	class TConstantBuffer : public T
	{
	public:
		TConstantBuffer() = default;
		TConstantBuffer(const T & inData) : T(inData) {}
		T& Data() { return *this; }

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

	class Buffer
	{
	public:
		template<EBufferType TBufferType, typename TDataType>
		static TUniquePtr<TBuffer<TDataType, TBufferType>> Create(const std::vector<TDataType>& inData)
		{
			switch (Renderer::API())
			{
			case ERenderAPI::OpenGL: return MakeUnique<TOpenGLBuffer<TBufferType, TDataType>>(inData);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}
	};

	class VertexBuffer
	{
	public:
		template<typename TDataType>
		static TUniquePtr<TVertexBuffer<TDataType>> Create(const std::vector<TDataType>& inData)
		{
			switch (Renderer::API())
			{
			case ERenderAPI::OpenGL: return MakeUnique<TOpenGLVertexBuffer<TDataType>>(inData);
			default:
				EV_CORE_ASSERT(false, "Unknown render API");
			}
			return nullptr;
		}
	};
}