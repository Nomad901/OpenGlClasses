#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::filesystem::path& pPath)
{
	Shader::ShaderProgramSource source = parseShader(pPath);
	mRendererID = createShader(source.mVertexSource, source.mFragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(mRendererID));
}

void Shader::bind() const
{
	GLCall(glUseProgram(mRendererID));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::setUniform1i(std::string_view pName, int32_t pValue)
{
	GLCall(glUniform1i(getUniformLocation(pName), pValue));	
}

void Shader::setUniform1f(std::string_view pName, float pValue)
{
	GLCall(glUniform1f(getUniformLocation(pName), pValue));
}

void Shader::setUniform4f(std::string_view pName, float pV0, float pV1, float pV2, float pV3)
{
	GLCall(glUniform4f(getUniformLocation(pName), pV0, pV1, pV2, pV3));
}

void Shader::setUniform4fv(std::string_view pName, const glm::mat4& pData)
{
	GLCall(glUniformMatrix4fv(getUniformLocation(pName), 1, GL_FALSE, &pData[0][0]));
}

Shader::ShaderProgramSource Shader::parseShader(const std::filesystem::path& pPath)
{
	std::ifstream ifstream(pPath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(ifstream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int32_t)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

uint32_t Shader::compileShaders(uint32_t pType, const std::string& pSource)
{
	GLCall(uint32_t id = glCreateShader(pType));
	const char* src = pSource.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int32_t result{};
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int32_t length{};
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << std::format("Failed to compile: {}\n Message: {}\n",
								 (pType == GL_VERTEX_SHADER ? "vertex" : "fragment"),
								  message);
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

uint32_t Shader::createShader(const std::filesystem::path& pVertexShader, const std::filesystem::path& pFragmentShader)
{
	GLCall(uint32_t program = glCreateProgram());
	uint32_t vs = compileShaders(GL_VERTEX_SHADER, pVertexShader.string());
	uint32_t fs = compileShaders(GL_FRAGMENT_SHADER, pFragmentShader.string());
	
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDetachShader(program, vs));
	GLCall(glDetachShader(program, fs));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	
	return program;
}

uint32_t Shader::getUniformLocation(std::string_view pName)
{
	if (mUniformLocationCache.contains(std::string(pName)))
		return mUniformLocationCache[std::string(pName)];

	GLCall(int32_t location = glGetUniformLocation(mRendererID, std::string(pName).c_str()));
	if (location == -1)
	{
		std::cout << std::format("The uniform was not found! Maybe a misleading name of the string. Name: {}\n", pName);
		return 0;
	}
	else
		mUniformLocationCache.insert_or_assign(std::string(pName), location);
	return location;
}
