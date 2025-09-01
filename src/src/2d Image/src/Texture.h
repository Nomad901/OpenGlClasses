#pragma once
#include <string>
#include <filesystem>

#include "stb_image.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "ErrorHandler.h"

class Texture
{
public:
	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;
	Texture operator=(const Texture&) = delete;

	void init(const std::filesystem::path& pPath);

	void setWidth(int32_t pWidth);
	void setHeight(int32_t pHeight);
	[[nodiscard]] int32_t getWidth() const noexcept;
	[[nodiscard]] int32_t getHeight() const noexcept;
	[[nodiscard]] std::filesystem::path getPath() const noexcept;

	void bind(uint32_t pSlot = 0) const;
	void unbind() const;

private:
	uint8_t* mLocalBuffer{ nullptr };
	uint32_t mRendererID{};
	int32_t mWidth{}, mHeight{}, mBPP{};
	std::filesystem::path mFilePath;

};

