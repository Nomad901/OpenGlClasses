#pragma once
#include <iostream>
#include <string>
#include <filesystem>

#include <glad/glad.h>

class Renderer;

class Texture
{
public:
	Texture() = default;
	Texture(GLenum pTextureTarget, std::string_view pTextureType, const std::filesystem::path& pFilePath);
	~Texture();

	void init(GLenum pTextureTarget, std::string_view pTextureType, const std::filesystem::path& pFilePath);

	void activeTex(uint32_t pSlot = 0) const;
	void bind() const;
	void unbind() const;
	
	int32_t getWidth() const noexcept;
	int32_t getHeight() const noexcept;
	int32_t getBPP() const noexcept;
	uint32_t getRendererID() const noexcept;
	std::string getTextureType() const noexcept;
	GLenum getTextureTarget() const noexcept;
	std::filesystem::path getFilePath() const noexcept;

private:
	int32_t mWidth{}, mHeight{}, mBPP{};
	uint32_t mRendererID{};
	uint8_t* mLocalBuffer{};

	GLenum mTextureTarget;
	std::string mType;

	std::filesystem::path mFilePath;
};

