#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>

#include "glad/glad.h"

#include "Shader.h"

class Renderer;

class Texture
{
public:
	Texture(const std::filesystem::path& pFilePath, std::string_view pType, uint32_t pSlot);
	~Texture();

	void texSlot(const Shader& pShader, std::string_view pUniform, GLuint pSlot);
	void bind() const;
	void unbind() const;
	
	int32_t getWidth() const noexcept;
	int32_t getHeight() const noexcept;
	int32_t getBPP() const noexcept;
	uint32_t getSlot() const noexcept;
	std::string getType() const noexcept;
	std::filesystem::path getFilePath() const noexcept;

private:
	int32_t mWidth{}, mHeight{}, mBPP{};
	uint32_t mRendererID{};
	uint32_t mSlot{};
	uint8_t* mLocalBuffer{};
	std::string mType;

	std::filesystem::path mFilePath;
};

