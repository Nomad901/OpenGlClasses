#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Renderer;

class Shader
{
private:
	struct ShaderProgramSource;
public:
	Shader(const std::filesystem::path& pPath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(std::string_view pName, int32_t pValue);
	void setUniform1f(std::string_view pName, float pValue);
	void setUniform4f(std::string_view pName, float pV0, float pV1, float pV2, float pV3);

private:
	ShaderProgramSource parseShader(const std::filesystem::path& pPath);
	uint32_t compileShaders(uint32_t pType, const std::string& pSource);
	uint32_t createShader(const std::filesystem::path& pVertexShader, const std::filesystem::path& pFragmentShader);
	uint32_t getUniformLocation(std::string_view pName);

private:
	struct ShaderProgramSource
	{
		std::filesystem::path mVertexSource;
		std::filesystem::path mFragmentSource;
	};

	uint32_t mRendererID{};
	std::filesystem::path mPath;
	std::unordered_map<std::string, int32_t> mUniformLocationCache;
};

