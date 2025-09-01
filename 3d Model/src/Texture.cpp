#include "Texture.h"
#include "Renderer.h"
#include "stb_image.h"

Texture::Texture(const std::filesystem::path& pFilePath, std::string_view pType, uint32_t pSlot)
	: mFilePath(pFilePath), mType(pType), mSlot(pSlot)
{
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(pFilePath.string().c_str(), &mWidth, &mHeight, &mBPP, 4);

	GLCall(glGenTextures(1, &mRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
	
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	if (mBPP == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer);
	else if (mBPP == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mLocalBuffer);
	else if (mBPP == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, mLocalBuffer);
	else
		std::cout << "Couldnt load a proper value for BPP(bytes per pixel) in Texture!\n";
	

	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (mLocalBuffer)
		stbi_image_free(mLocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mRendererID));
}

void Texture::texSlot(const Shader& pShader, std::string_view pUniform, GLuint pSlot)
{
	std::string tmpUniformName = std::string(pUniform);
	GLuint texUni = glGetUniformLocation(pShader.getId(), tmpUniformName.c_str());
	pShader.bind();
	GLCall(glUniform1i(texUni, pSlot));
}

void Texture::bind() const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + mSlot));
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

uint32_t Texture::getSlot() const noexcept
{
	return mSlot;
}

std::string Texture::getType() const noexcept
{
	return mType;
}

std::filesystem::path Texture::getFilePath() const noexcept
{
	return mFilePath;
}
