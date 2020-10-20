#include "PCH.h"
#include "Material.h"

namespace Evoke
{
	ShaderLibrary* Material::gShaderLibrary = new ShaderLibrary();

	Material::Material(StringView inShader) : mName(Filesystem::Filename(inShader))
	{
		mShader = &(*gShaderLibrary->Load(inShader));
		mConstantBuffer = ConstantBuffer::Create<MaterialBuffer>(mSlot);
	}

	Material::Material(StringView inShader, const ShaderCompilerConfig& inConfig) : mName(Filesystem::Filename(inShader))
	{
		mShader = &(*gShaderLibrary->Load(inShader, inConfig));
		mConstantBuffer = ConstantBuffer::Create<MaterialBuffer>(mSlot);
	}

	void Material::Bind()
	{
		mShader->Bind();
		if (mIsDirty)
			mConstantBuffer->Upload();
		mConstantBuffer->SetSlot(mSlot);
	}

	void Material::SetTextureParameter(i32 inIndex, Texture* inTexture)
	{
		mTextures[inIndex] = inTexture;
	}
}

