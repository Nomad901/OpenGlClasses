#include "Texture.h"
#include "Renderer.h"
#include "stb_image.h"

Texture::Texture(GLenum pTextureTarget, std::string_view pTextureType, const std::filesystem::path& pFilePath)
	: mFilePath(pFilePath), mType(pTextureType)
{
	init(pTextureTarget, pTextureType, pFilePath);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mRendererID));
}

void Texture::init(GLenum pTextureTarget, std::string_view pTextureType, const std::filesystem::path& pFilePath)
{
	mTextureTarget = pTextureTarget;
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pFilePath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);
	if (!mLocalBuffer)
	{
		std::cout << std::format("Couldnt download the local buffer. Reason: {}\n", stbi_failure_reason());
		return;
	}

	GLCall(glGenTextures(1, &mRendererID));
	GLCall(glBindTexture(pTextureTarget, mRendererID));

	GLCall(glTexParameteri(pTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(pTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(pTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(pTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(pTextureTarget, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));
	GLCall(glBindTexture(pTextureTarget, 0));

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

void Texture::bind(uint32_t pSlot) const
{
	glBindTextureUnit(pSlot, mRendererID);
}

void Texture::unbind() const
{
	GLCall(glBindTexture(mTextureTarget, 0));
}

int32_t Texture::getWidth() const noexcept
{
	return mWidth;
}

int32_t Texture::getHeight() const noexcept
{
	return mHeight;
}

int32_t Texture::getBPP() const noexcept
{
	return mBPP;
}

uint32_t Texture::getRendererID() const noexcept
{
	return mRendererID;
}

std::string Texture::getTextureType() const noexcept
{
	return mType;
}

GLenum Texture::getTextureTarget() const noexcept
{
	return mTextureTarget;
}

std::filesystem::path Texture::getFilePath() const noexcept
{
	return mFilePath;
}
