#pragma once

#include "glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

class Mesh3D
{
public:
	Mesh3D(const std::vector<Vertex>& pVertexes, 
		   const std::vector<GLuint>& pIndicies,
		   const std::vector<Texture>& pTextures);

	void draw(Shader& pShader,
			  Camera& pCamera,
			  const glm::mat4& pMatrix = glm::mat4(1.0f),
			  const glm::quat& pRotate = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			  const glm::vec3& pTranslate = glm::vec3(0.0f, 0.0f, 0.0f),
			  const glm::vec3& pScale = glm::vec3(1.0f, 1.0f, 1.0f));

public:
	VertexArray mVAO;

	std::vector<Vertex> mVertexes;
	std::vector<GLuint> mIndicies;
	std::vector<Texture	> mTextures;
};

