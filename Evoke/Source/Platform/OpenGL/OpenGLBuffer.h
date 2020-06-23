#pragma once
#include "Core/Core.h"
#include "Renderer/Buffer.h"
#include <glad/glad.h>

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
		virtual void Update() override
		{
			glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &mData);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		virtual u32 GetSlot() override { return mSlot; }
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
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &mData, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, mSlot, mBufferID);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	private:
		u32 mBufferID;
		u32 mSlot;
	};
}