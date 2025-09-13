#pragma once
#include <iostream>
#include <filesystem>
#include <string>

#include "glad/glad.h"
#include "stb_image.h"

#include "Material.h"

class Texture2
{
public:
	Texture2() = default;
	Texture2(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture = false);
	~Texture2();

	void init(const std::filesystem::path& pPath, std::string_view pUniformName, bool pRepeatTexture = false);

	void bind(uint32_t pSlot = 0);
	void unbind();

	uint32_t getRendererId() const noexcept;
	std::string getUniformName() const noexcept;

	void setID(uint32_t pID);
	void setType(std::string_view pType);
	void setPath(const std::filesystem::path& pPath);

	uint32_t getID() const noexcept;
	std::string getType() const noexcept;
	std::filesystem::path getPath() const noexcept;

private:
	int32_t mWidth{}, mHeight{}, mBPP{};
	uint32_t mRendererID{};
	uint8_t* mLocalBuffer{};

	std::string mType;

	std::string mUniformName;
	std::filesystem::path mFilePath;
};

