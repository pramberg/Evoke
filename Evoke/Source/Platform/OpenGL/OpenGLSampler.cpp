#include "PCH.h"
#include "OpenGLSampler.h"
#include "OpenGLUtilities.h"

#include <glad\glad.h>

namespace Evoke
{
	OpenGLSampler::OpenGLSampler()
	{
		glCreateSamplers(1, &mSamplerID);
	}

	OpenGLSampler::OpenGLSampler(const SamplerDescription& inDescription) : OpenGLSampler()
	{
		UpdateDescription(inDescription);
	}

	OpenGLSampler::~OpenGLSampler()
	{
		glDeleteSamplers(1, &mSamplerID);
	}

	void OpenGLSampler::UpdateDescription(const SamplerDescription& inDescription)
	{
		const GLenum addressMode = OpenGLUtilities::ConvertAddressMode(inDescription.AddressMode);
		glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_S, addressMode);
		glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_T, addressMode);
		glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_R, addressMode);
		glSamplerParameteri(mSamplerID, GL_TEXTURE_MIN_FILTER, OpenGLUtilities::ConvertMinFilter(inDescription.TextureFilter));
		glSamplerParameteri(mSamplerID, GL_TEXTURE_MAG_FILTER, OpenGLUtilities::ConvertMagFilter(inDescription.TextureFilter));
		glSamplerParameterf(mSamplerID, GL_TEXTURE_MAX_ANISOTROPY, (f32)inDescription.AnisotropicLevels);
		glSamplerParameterfv(mSamplerID, GL_TEXTURE_BORDER_COLOR, &inDescription.BorderColor.x);
	}

	void OpenGLSampler::Bind(u32 inSlot)
	{
		glBindSampler(inSlot, mSamplerID);
	}

	void OpenGLSampler::Unbind(u32 inSlot)
	{
		glBindSampler(inSlot, mSamplerID);
	}

}