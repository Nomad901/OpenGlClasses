#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

class Texture2;

class Material
{
public:
	Material() = default;
	Material(glm::vec3 pAmbient,
			 glm::vec3 pDiffuse,
			 glm::vec3 pSpecular,
			 std::vector<Texture2>& pTextures);

	void init(glm::vec3 pAmbient,
			  glm::vec3 pDiffuse,
			  glm::vec3 pSpecular,
			  std::vector<Texture2>& pTextures);

	void sendToShader(Shader& pShader, bool isJustColored = false);

private:
	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
	std::vector<Texture2> mTextures;

};

