#pragma once
#include <iostream>
#include <string>
#include <filesystem>

class Renderer;

class Texture
{
public:
	Texture(const std::filesystem::path& pFilePath);
	~Texture();

	void bind(uint32_t pSlot = 0) const;
	void unbind() const;
	
	int32_t getWidth() const noexcept;
	int32_t getHeight() const noexcept;
	int32_t getBPP() const noexcept;
	std::filesystem::path getFilePath() const noexcept;

private:
	int32_t mWidth{}, mHeight{}, mBPP{};
	uint32_t mRendererID{};
	uint8_t* mLocalBuffer{};

	std::filesystem::path mFilePath;
};

