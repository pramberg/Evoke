#pragma once
#include "Core/Core.h"
#include "Renderer/Buffer.h"
#include <glad/glad.h>

#include <type_traits>
#include "boost/pfr/precise/core.hpp"

namespace Evoke
{
	template<typename T>
	class TOpenGLConstantBuffer : public TConstantBuffer<T>
	{
	public:
		TOpenGLConstantBuffer(const T& inData, u32 inSlot) : TConstantBuffer(inData), mSlot(inSlot)
		{
			Initialize();
		}

		TOpenGLConstantBuffer(u32 inSlot) : TConstantBuffer(), mSlot(inSlot)
		{
			Initialize();
		}

		~TOpenGLConstantBuffer()
		{
			glDeleteBuffers(1, &mBufferID);
		}

	public:
		// #TODO: This is probably very naive and inefficient. Use glBufferSubData() but only update values that were updated.
		virtual void Upload() override
		{
			glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), static_cast<T*>(this));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		virtual u32 Slot() override { return mSlot; }
		virtual void SetSlot(u32 inSlot) override
		{
			glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
			glBindBufferBase(GL_UNIFORM_BUFFER, inSlot, mBufferID);
			mSlot = inSlot;
		}

	private:
		void Initialize()
		{
			glCreateBuffers(1, &mBufferID);
			glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), static_cast<T*>(this), GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, mSlot, mBufferID);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	private:
		u32 mBufferID;
		u32 mSlot;
	};

	static constexpr GLenum ConvertToOpenGLType(EBufferType inBufferType)
	{
		switch (inBufferType)
		{
		case EBufferType::Vertex: return GL_ARRAY_BUFFER;
		case EBufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
		case EBufferType::Constant: return GL_UNIFORM_BUFFER;
		default:
			return GL_NONE;
		}
	}

	template<EBufferType TBufferType, typename TDataType>
	class TOpenGLBuffer : public TBuffer<TDataType, TBufferType>
	{
	public:
		TOpenGLBuffer(const std::vector<TDataType>& inData) : TBuffer<TDataType, TBufferType>()
		{
			glCreateBuffers(1, &mBufferID);
			Bind();
			glBufferData(ConvertToOpenGLType(TBufferType), inData.size() * sizeof(TDataType), inData.data(), GL_STATIC_DRAW);
			
		}

		virtual void Bind() override
		{
			glBindBuffer(ConvertToOpenGLType(TBufferType), mBufferID);
			if (TBufferType == EBufferType::Vertex)
			{
				glBindVertexBuffer(0, mBufferID, 0, sizeof(TDataType));
			}
		}

		virtual void SetData(const std::vector<TDataType>& inData) override
		{
			Bind();
			glBufferSubData(ConvertToOpenGLType(TBufferType), 0, inData.size() * sizeof(TDataType), inData.data());
		}

	private:

		u32 mBufferID;
	};



#define MAKE_HAS_MEMBER(inMember)                                            \
	template <typename T>													 \
	struct HasMember_##inMember												 \
	{																		 \
		using yes = char[1];												 \
		using no = char[2];													 \
		template <typename C> static yes& test(typename C::##inMember*);	 \
		template <typename> static no& test(...);							 \
		static constexpr b8 value = sizeof(test<T>(nullptr)) == sizeof(yes); \
	}                                          								\

	MAKE_HAS_MEMBER(value_type);
	MAKE_HAS_MEMBER(length);

	template <class>
	_INLINE_VAR constexpr bool IsValidArrayType = false; // determine whether type argument is an array

	template <class _Ty, size_t _Nx>
	_INLINE_VAR constexpr bool IsValidArrayType<std::array<_Ty, _Nx>> = true;

	template <class _Ty>
	_INLINE_VAR constexpr bool IsValidArrayType<std::array<_Ty, 0>> = true;

	template<typename T> constexpr b8 IsFundamentallyFloatingPoint()
	{
		if constexpr (IsValidArrayType<T>)
			return IsFundamentallyFloatingPoint<typename T::value_type>();
		else if constexpr (HasMember_value_type<T>::value)
			return std::is_floating_point_v<typename T::value_type>;
		else
			return std::is_floating_point_v<T>;
	}

	template<typename T> constexpr b8 IsFundamentallyIntegral()
	{
		if constexpr (IsValidArrayType<T>)
			return IsFundamentallyIntegral<typename T::value_type>();
		else if constexpr (HasMember_value_type<T>::value)
			return std::is_integral_v<typename T::value_type>;
		else
			return std::is_integral_v<T>;
	}

	template<typename T> constexpr size_t FundamentalSize()
	{
		using CurrentType = typename std::decay_t<T>;
		if constexpr (HasMember_value_type<CurrentType>::value)
			return FundamentalSize<CurrentType::value_type>();
		else if constexpr (std::is_fundamental_v<CurrentType>)
			return sizeof(CurrentType);
		else
			static_assert(false, "Type must be fundamental or provide a 'value_type' typedef to discover the type it is based on.");
	}

	template<typename T> constexpr size_t NumIterations()
	{
		using CurrentType = typename std::decay_t<T>;
		if constexpr (IsValidArrayType<CurrentType>)
			return std::size<CurrentType>({});
		else
			return 1;
	}

	// Recursively calculate the number of basic member variables, "unrolling" arrays and #TODO: matrices.
	template<typename T, size_t N = boost::pfr::tuple_size<T>::value>
	struct TotalCount
	{
		using MemberType = decltype(boost::pfr::get<N - 1>(T{}));
		static constexpr size_t value = NumIterations<MemberType>() + TotalCount<T, N - 1>::value;
	};

	template<typename T>
	struct TotalCount<T, 1>
	{
		using MemberType = decltype(boost::pfr::get<0>(T{}));
		static constexpr size_t value = NumIterations<MemberType>();
	};

	template<typename T> constexpr size_t ComponentCount()
	{
		using CurrentType = typename std::decay_t<T>;
		constexpr size_t currentTypeSize = sizeof(CurrentType);
		constexpr size_t baseTypeSize = FundamentalSize<CurrentType>();
		constexpr u8 numComponents = currentTypeSize / baseTypeSize;

		if constexpr (IsValidArrayType<CurrentType>)
			return numComponents / std::size<CurrentType>({});
		else
			return numComponents;
	}

	
	/**
	 * Unpacks a struct and generates the EShaderDataTypes required to fill out a vertex array object.
	 * @tparam	TVertex	The struct that the vertex buffer uses to define its data.
	 * @returns	An std::array of EShaderDataTypes.
	 */
	template<typename TVertex>
	constexpr std::array<EShaderDataType, TotalCount<TVertex>::value> ShaderDataTypes()
	{
		std::array<EShaderDataType, TotalCount<TVertex>::value> outArray{};
		i32 index = 0;
		// #TODO: This is not constexpr, but I believe it could be done at compile time.
		boost::pfr::for_each_field<TVertex>({}, [&outArray, &index](auto& inOutField)
		{
			using CurrentType = typename std::decay_t<decltype(inOutField)>;
			constexpr size_t numComponents = ComponentCount<CurrentType>();
			static_assert(numComponents <= 4, "OpenGL only supports types with max 4 components.");
			for (i32 i = 0; i < NumIterations<CurrentType>(); i++)
			{
				if constexpr (IsFundamentallyFloatingPoint<CurrentType>())
				{
					outArray[index++] = static_cast<EShaderDataType>((i32)EShaderDataType::Float + numComponents - 1);
				}
				else if constexpr (IsFundamentallyIntegral<CurrentType>())
				{
					outArray[index++] = static_cast<EShaderDataType>((i32)EShaderDataType::Int + numComponents - 1);
				}
			}
		});
		return outArray;
	}

	constexpr GLenum ConvertToOpenGLDataType(EShaderDataType inShaderDataType)
	{
		switch (inShaderDataType)
		{
		case Evoke::EShaderDataType::Float:
		case Evoke::EShaderDataType::Float2:
		case Evoke::EShaderDataType::Float3:
		case Evoke::EShaderDataType::Float4:
		case Evoke::EShaderDataType::Float2x2:
		case Evoke::EShaderDataType::Float2x3:
		case Evoke::EShaderDataType::Float3x2:
		case Evoke::EShaderDataType::Float3x3:
		case Evoke::EShaderDataType::Float3x4:
		case Evoke::EShaderDataType::Float4x3:
		case Evoke::EShaderDataType::Float4x4:
		case Evoke::EShaderDataType::Float4x2:
		case Evoke::EShaderDataType::Float2x4:
			return GL_FLOAT;
		case Evoke::EShaderDataType::Int:
		case Evoke::EShaderDataType::Int2:
		case Evoke::EShaderDataType::Int3:
		case Evoke::EShaderDataType::Int4:
			return GL_INT;
		case Evoke::EShaderDataType::Bool:
			return GL_BOOL;
		case Evoke::EShaderDataType::Bool2:
			return GL_BOOL_VEC2;
		case Evoke::EShaderDataType::Bool3:
			return GL_BOOL_VEC3;
		case Evoke::EShaderDataType::Bool4:
			return GL_BOOL_VEC4;
		case Evoke::EShaderDataType::Invalid:
		default:
			EV_ASSERT(false, "Invalid shader data type.");
			return GL_NONE;
		}
	}

	template<typename TDataType>
	class TOpenGLVertexBuffer : public TVertexBuffer<TDataType>
	{
	public:
		TOpenGLVertexBuffer(const std::vector<TDataType>& inData) : TVertexBuffer<TDataType>()
		{
			glCreateVertexArrays(1, &mVertexArrayID);
			glBindVertexArray(mVertexArrayID);

			glCreateBuffers(1, &mBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
			glBindVertexBuffer(0, mBufferID, 0, sizeof(TDataType));
			glBufferData(GL_ARRAY_BUFFER, inData.size() * sizeof(TDataType), inData.data(), GL_STATIC_DRAW);

			i32 sumStride = 0;
			auto shaderTypes = ShaderDataTypes<TDataType>();
			for (i32 i = 0; i < shaderTypes.size(); i++)
			{
				glVertexAttribFormat(i, ShaderDataTypeComponentCount(shaderTypes[i]), ConvertToOpenGLDataType(shaderTypes[i]), GL_FALSE, sumStride);
				glVertexAttribBinding(i, 0);
				glEnableVertexAttribArray(i);
				sumStride += ShaderDataTypeSize(shaderTypes[i]);
			}
		}

		virtual ~TOpenGLVertexBuffer() override
		{
			glDeleteBuffers(1, &mBufferID);
			glDeleteVertexArrays(1, &mVertexArrayID);
		}

		virtual void Bind() override
		{
			glBindVertexArray(mVertexArrayID);
		}

		virtual void SetData(const std::vector<TDataType>& inData) override
		{
			glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
			glBindVertexBuffer(0, mBufferID, 0, sizeof(TDataType));
			glBufferSubData(GL_ARRAY_BUFFER, 0, inData.size() * sizeof(TDataType), inData.data());
		}

	private:
		u32 mBufferID;
		u32 mVertexArrayID;
	};
}