#include "ShadersSys.h"

void ShadersSys::init(GLuint& pGrpahicsPipelineProgram,
					  const std::filesystem::path& pVertexShaderPath,
					  GLuint pTypeVertex, 
					  const std::filesystem::path& pFragmentShaderPath,
					  GLuint pTypeFragment)
{
	const std::string vertexShader = getStringFromFile(pVertexShaderPath);
	const std::string fragmentShader = getStringFromFile(pFragmentShaderPath);

	pGrpahicsPipelineProgram = createShaders(vertexShader, pTypeVertex, fragmentShader, pTypeFragment);
}

std::string ShadersSys::getStringFromFile(const std::filesystem::path& pPath)
{
	std::ifstream file(pPath);
	if (file.is_open() && std::filesystem::exists(pPath))
	{
		std::string finalStr;
		std::string line;
		while (std::getline(file, line))
		{
			finalStr += line + '\n';
		}
		return finalStr;
	}
	mErrorHandler.log(type::ERROR, "Be it file cant be opened or the file doesnt exist (?)", __FILE__, __LINE__);
	return "";
}

GLuint ShadersSys::createShaders(std::string_view pVertexShaderSource, 
								 GLuint pTypeVertex,
								 std::string_view pFragmentShaderSource,
								 GLuint pTypeFragment)
{
	GLuint programObj = glCreateProgram();
	GLuint vertexShader = compileShaders(GL_VERTEX_SHADER, pVertexShaderSource);
	GLuint fragmentShader = compileShaders(GL_FRAGMENT_SHADER, pFragmentShaderSource);

	glAttachShader(programObj, vertexShader);
	glAttachShader(programObj, fragmentShader);

	glLinkProgram(programObj);
	glValidateProgram(programObj);

	glDetachShader(programObj, vertexShader);
	glDetachShader(programObj, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return programObj;
}

GLuint ShadersSys::compileShaders(GLuint pType, std::string_view pShaderSource)
{
	GLuint shaderObject = 0;

	if (pType == GL_VERTEX_SHADER)
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	else if (pType == GL_FRAGMENT_SHADER)
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		mErrorHandler.log(type::CRITICAL_ERROR, "There is no such a type!", __FILE__, __LINE__);
		std::terminate();
	}

	std::string sourceStr(pShaderSource);
	const char* str = sourceStr.c_str();
	glShaderSource(shaderObject, 1, &str, nullptr);
	glCompileShader(shaderObject);

	int32_t result = 0;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int32_t length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		char* msg = new char[length];
		glGetShaderInfoLog(shaderObject, length, &length, msg);

		if (pType == GL_VERTEX_SHADER)
			mErrorHandler.log(type::CRITICAL_ERROR, "Couldnt compile the vertex shader!", __FILE__, __LINE__);
		else if (pType == GL_FRAGMENT_SHADER)
			mErrorHandler.log(type::CRITICAL_ERROR, "Couldnt compile the fragment shader!\n", __FILE__, __LINE__);
		
		delete[] msg;
		glDeleteShader(shaderObject);
		return 0;
	}
	return shaderObject;
}
