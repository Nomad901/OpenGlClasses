#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& pVertices, 
		   const std::vector<uint32_t>& pIndices, 
		   const std::vector<Texture2>& pTexture)
{
	init(pVertices, pIndices, pTexture);
}

Mesh::Mesh(std::unique_ptr<Primitive>& pPrimitive,
		   const std::vector<Texture2>& pTexture)
{
	init(pPrimitive, pTexture);
}

Mesh::Mesh(std::unique_ptr<Primitive>& pPrimitive)
{
	init(pPrimitive);
}

void Mesh::init(const std::vector<Vertex>& pVertices,
				const std::vector<uint32_t>& pIndices,
				const std::vector<Texture2>& pTextures)
{
	mPrimitive = std::make_unique<Primitive>();
	
	mVAO.generate();
	mVAO.bind();
	mVBO.init(pVertices, GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(pIndices.data(), pIndices.size());
	mTextures = pTextures;
	mPrimitive->setVertexStrg(pVertices);
	mPrimitive->setIndexStrg(pIndices);
}

void Mesh::init(std::unique_ptr<Primitive>& pPrimitive,
				const std::vector<Texture2>& pTextures)
{
	mPrimitive = std::move(pPrimitive);

	mVAO.generate();
	mVAO.bind();
	mVBO.init(mPrimitive->getVertexStrg(), GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(mPrimitive->getIndexStrg().data(), mPrimitive->getIndexStrg().size());
	mTextures = pTextures;
}

void Mesh::init(std::unique_ptr<Primitive>& pPrimitive)
{
	mPrimitive = std::move(pPrimitive);

	mVAO.generate();
	mVAO.bind();
	mVBO.init(mPrimitive->getVertexStrg(), GL_STATIC_DRAW);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 3);
	mVBOLayout.pushLayout(GL_FLOAT, 4);
	mVBOLayout.pushLayout(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(mPrimitive->getIndexStrg().data(), mPrimitive->getIndexStrg().size());
}

void Mesh::initMVP(int32_t pWinWidth, int32_t pWinHeight, const glm::mat4& pViewMatrix,
				   const glm::vec3& pTranslation, const std::pair<float, glm::vec3>& pDegreeRotate,
				   const glm::vec3& pScale)
{
	mProjMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::mat4(1.0f);

	mProjMatrix = glm::perspective(glm::radians(45.0f), (float)pWinWidth / (float)pWinHeight, 0.1f, 200.0f);
	mViewMatrix = pViewMatrix;
	mModelMatrix = glm::translate(mModelMatrix, pTranslation);
	mModelMatrix = glm::rotate(mModelMatrix, glm::radians(pDegreeRotate.first), pDegreeRotate.second);
	mModelMatrix = glm::scale(mModelMatrix, pScale);
	mMVP = mProjMatrix * mViewMatrix * mModelMatrix;
}

void Mesh::setModelMatrix(const glm::mat4& pModelMatrix)
{
	mModelMatrix = pModelMatrix;
}

void Mesh::setViewMatrix(const glm::mat4& pViewMatrix)
{
	mViewMatrix = pViewMatrix;
}

void Mesh::setProjMatrix(const glm::mat4& pProjMatrix)
{
	mProjMatrix = pProjMatrix;
}

void Mesh::setMVP(const glm::mat4& pModelMatrix, 
				  const glm::mat4& pViewMatrix, 
				  const glm::mat4& pProjMatrix)
{
	mModelMatrix = pModelMatrix;
	mViewMatrix = pViewMatrix;
	mProjMatrix = pProjMatrix;
}

void Mesh::setVertices(const std::vector<Vertex>& pVertices)
{
	mPrimitive->setVertexStrg(pVertices);
}

void Mesh::setIndices(const std::vector<uint32_t>& pIndices)
{
	mPrimitive->setIndexStrg(pIndices);
}

void Mesh::setTextures(const std::vector<Texture2>& pTextures)
{
	mTextures = pTextures;
}

std::vector<Vertex>& Mesh::getVertices()
{
	return mPrimitive->getVertexStrg();
}

std::vector<uint32_t>& Mesh::getIndices()
{
	return mPrimitive->getIndexStrg();
}

std::vector<Texture2>& Mesh::getTextures()
{
	return mTextures;
}

Primitive& Mesh::getPrimitive()
{
	return *mPrimitive.get();
}

glm::mat4 Mesh::getModelMatrix() const noexcept
{
	return mModelMatrix;
}

glm::mat4 Mesh::getViewMatrix() const noexcept
{
	return mViewMatrix;
}

glm::mat4 Mesh::getProjMatrix() const noexcept
{
	return mProjMatrix;
}

glm::mat4 Mesh::getMVP(bool pWithComputations)
{
	if(pWithComputations)
		mMVP = mProjMatrix * mViewMatrix * mModelMatrix;
	return mMVP;
}

VAO Mesh::getVAO() const noexcept
{
	return mVAO;
}

VBO Mesh::getVBO() const noexcept
{
	return mVBO;
}

EBO Mesh::getEBO() const noexcept
{
	return mEBO;
}

VBOLayout Mesh::getVBOLayout() const noexcept
{
	return mVBOLayout;
}

void Mesh::draw()
{
	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		mTextures[i].bind(i);
	}
	mVAO.bind();
	glDrawElements(GL_TRIANGLES, mEBO.getCount(), GL_UNSIGNED_INT, nullptr);
	for (auto& i : mTextures)
	{
		i.unbind();
	}
	mVAO.unbind();
}

void Mesh::drawForModels(Shader& pShader)
{
	uint32_t diffuseNr = 0;
	uint32_t specularNr = 0;
	uint32_t heightNr = 0;
	uint32_t normalNr = 0;
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = mTextures[i].getType();
		if (name == "texture_diffuse")
			number = std::to_string(++diffuseNr);
		else if (name == "texture_specular")
			number = std::to_string(++specularNr);
		else if (name == "texture_height")
			number = std::to_string(++heightNr);
		else if (name == "texture_normal")
			number = std::to_string(++normalNr);
		glUniform1i(pShader.getUniformLocation((name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i].getID());
	}

	mVAO.bind();
	for (auto& i : mTextures)
	{
		i.bind();
	}
	glDrawElements(GL_TRIANGLES, mPrimitive->getIndexStrg().size(), GL_UNSIGNED_INT, nullptr);

	glActiveTexture(GL_TEXTURE0);
}
