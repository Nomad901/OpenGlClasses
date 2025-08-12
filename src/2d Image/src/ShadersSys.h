#pragma once
#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "glad/glad.h"

#include "ErrorHandler.h"

class ShadersSys
{
public:
	void init(GLuint& pGrpahicsPipelineProgram,
			  const std::filesystem::path& pVertexShaderPath, 
			  GLuint pTypeVertex,
			  const std::filesystem::path& pFragmentShaderPath, 
			  GLuint pTypeFragment);
	std::string getStringFromFile(const std::filesystem::path& pPath);

private:
	GLuint createShaders(std::string_view pVertexShaderSource, GLuint pTypeVertex,
						 std::string_view pFragmentShaderSource, GLuint pTypeFragment);
	GLuint compileShaders(GLuint pType, std::string_view pShaderSource);

};

