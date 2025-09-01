#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& pVertices, const std::vector<uint32_t>& pIndexes, std::vector<Texture> pTextures)
	: mVertices(pVertices),
	  mIndexes(pIndexes),
	  mTextures(std::move(pTextures))
{
	setupMesh();
}

void Mesh::draw(Shader& pShader)
{
	pShader.bind();
	uint32_t diffuseNum = 1;
	uint32_t specularNum = 1;
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		mTextures[i].activeTex(i);
		std::string number;
		std::string name = mTextures[i].getTextureType();
		if (name == "uTexture_diffuse")
			number = std::to_string(diffuseNum++);
		else if (name == "uTexture_specular")
			number = std::to_string(specularNum++);
		pShader.setUniform1i((name + number), i);
		mTextures[i].bind();
	}
	GLCall(glActiveTexture(GL_TEXTURE0));

	mRenderer.draw(mVAO, mEBO, pShader);
}

std::vector<Vertex>& Mesh::getVertices() noexcept
{
	return mVertices;
}

std::vector<uint32_t>& Mesh::getIndexes() noexcept
{
	return mIndexes;
}

std::vector<Texture>& Mesh::getTextures() noexcept
{
	return mTextures;
}

void Mesh::setupMesh()
{
	mVAO.bind();
	mVBO.init(mVertices);
	
	mVertexBufferLayout.push(GL_FLOAT, 3);
	mVertexBufferLayout.push(GL_FLOAT, 3);
	mVertexBufferLayout.push(GL_FLOAT, 2);
	mVAO.addBuffer(mVBO, mVertexBufferLayout);

	mEBO.init(mIndexes.data(), mIndexes.size() * sizeof(uint32_t));
}
