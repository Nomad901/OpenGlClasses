#pragma once
#include <iostream>
#include <memory>

#include "glm.hpp"
#include "VBO.h"
#include "VAO.h"
#include "VBOLayout.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture2.h"
#include "Primitive.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::vector<Vertex>& pVertices,
		 const std::vector<uint32_t>& pIndices,
		 const std::vector<Texture2>& pTexture);
	Mesh(std::unique_ptr<Primitive>& pPrimitive,
		 const std::vector<Texture2>& pTexture);
	Mesh(std::unique_ptr<Primitive>& pPrimitive);

	void init(const std::vector<Vertex>& pVertices,
			  const std::vector<uint32_t>& pIndices, 
			  const std::vector<Texture2>& pTexture);
	void init(std::unique_ptr<Primitive>& pPrimitive,
			  const std::vector<Texture2>& pTextures);
	void init(std::unique_ptr<Primitive>& pPrimitive);

	void initMVP(int32_t pWinWidth, int32_t pWinHeight, const glm::mat4& pViewMatrix,
				 const glm::vec3& pTranslation, const std::pair<float, glm::vec3>& pDegreeRotate,
				 const glm::vec3& pScale);
	void setModelMatrix(const glm::mat4& pModelMatrix);
	void setViewMatrix(const glm::mat4& pViewMatrix);
	void setProjMatrix(const glm::mat4& pProjMatrix);
	void setMVP(const glm::mat4& pModelMatrix,
				const glm::mat4& pViewMatrix,
				const glm::mat4& pProjMatrix);
	glm::mat4 getModelMatrix() const noexcept;
	glm::mat4 getViewMatrix() const noexcept;
	glm::mat4 getProjMatrix() const noexcept;
	glm::mat4 getMVP(bool pWithComputations);

	void setVertices(const std::vector<Vertex>& pVertices);
	void setIndices(const std::vector<uint32_t>& pIndices);
	void setTextures(const std::vector<Texture2>& pTextures);
	std::vector<Vertex>& getVertices();
	std::vector<uint32_t>& getIndices();
	std::vector<Texture2>& getTextures();
	Primitive& getPrimitive();

	VAO getVAO() const noexcept;
	VBO getVBO() const noexcept;
	EBO getEBO() const noexcept;
	VBOLayout getVBOLayout() const noexcept;

	//
	// pNameUniform - name for MVP uniform in shaders
	//
	void draw();
	void drawForModels(Shader& pShader);

private:
	std::unique_ptr<Primitive> mPrimitive;
	std::vector<Texture2> mTextures;

	VAO mVAO;
	VBO mVBO;
	EBO mEBO;
	VBOLayout mVBOLayout;

	glm::mat4 mModelMatrix{ 1.0f }, mViewMatrix{ 1.0f }, mProjMatrix{ 1.0f };
	glm::mat4 mMVP{ 1.0f };
};

