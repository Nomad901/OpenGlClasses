#include "Texture.h"
#include "Renderer.h"
#include "stb_image.h"

Texture::Texture(const std::filesystem::path& pFilePath)
	: mFilePath(pFilePath)
{
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pFilePath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);

	GLCall(glGenTextures(1, &mRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mRendererID));
}

void Texture::bind(uint32_t pSlot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + pSlot));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
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

std::filesystem::path Texture::getFilePath() const noexcept
{
	return mFilePath;
}
