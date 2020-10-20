#pragma once
#include "Core/Core.h"
#include "Buffer.h"
#include "ShaderLibrary.h"
#include "GraphicsContext.h"
#include "Texture.h"

namespace Evoke
{
	class Shader;

	struct MaterialBuffer
	{
		std::array<f32, 128> Vectors;
	};

	struct TextureParameter
	{
		u32 Index;
		class Texture* Texture;
	};

	class Material
	{
	public:
		Material(StringView inShader);
		Material(StringView inShader, const ShaderCompilerConfig& inConfig);

		void Bind();

		template<typename T>
		void SetParameter(i32 inIndex, const T& inValue)
		{
			EV_ASSERT((inIndex + sizeof(T)) < sizeof(mConstantBuffer->Data()), "Out of bounds.");
			memcpy(mConstantBuffer->Vectors.data() + inIndex, (f32*)&inValue, sizeof(T));
			mIsDirty = true;
		}

		void SetTextureParameter(i32 inIndex, Texture* inTexture);

		void SetFaceCulling(EFaceCulling inFaceCulling) { mFaceCulling = inFaceCulling; }
		EFaceCulling FaceCulling() const { return mFaceCulling; }

		const std::map<i32, Texture*>& Textures() const { return mTextures; }
		std::map<i32, Texture*>& Textures() { return mTextures; }

		EV_NODISCARD String Name() const { return mName; }

	private:
		Shader* mShader = nullptr;
		TUniquePtr<TConstantBuffer<MaterialBuffer>> mConstantBuffer = nullptr;
		b8 mIsDirty = true;
		EFaceCulling mFaceCulling = EFaceCulling::Back;
		std::map<i32, Texture*> mTextures;
		String mName;

		static constexpr u32 mSlot = 1;
		friend class Application;
		
		static ShaderLibrary* gShaderLibrary;
	};
}