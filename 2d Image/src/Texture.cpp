#include "Texture.h"

Texture::~Texture()
{
	glDeleteTextures(1, &mRendererID);
}

void Texture::init(const std::filesystem::path& pPath)
{
	mFilePath = pPath;

	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pPath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);
	if (!mLocalBuffer)
		LOG(type::ERROR, "Couldnt initialize the local buffer!");

	glGenTextures(1, &mRendererID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mRendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

void Texture::setWidth(int32_t pWidth)
{
	mWidth = pWidth;
}

void Texture::setHeight(int32_t pHeight)
{
	mHeight = pHeight;
}

int32_t Texture::getWidth() const noexcept
{
	return mWidth;
}

int32_t Texture::getHeight() const noexcept
{
	return mHeight;
}

std::filesystem::path Texture::getPath() const noexcept
{
	return mFilePath;
}

void Texture::bind(uint32_t pSlot) const
{
	glActiveTexture(GL_TEXTURE0 + pSlot);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

