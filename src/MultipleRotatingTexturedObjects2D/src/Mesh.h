#pragma once
#include <iostream>
#include <format>
#include <filesystem>

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferOrig.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& pVertices, const std::vector<uint32_t>& pIndexes,
	     std::vector<Texture> pTextures);

	void draw(Shader& pShader);

	std::vector<Vertex>& getVertices() noexcept;
	std::vector<uint32_t>& getIndexes() noexcept;
	std::vector<Texture>& getTextures() noexcept;

private:
	void setupMesh();

private:
	VertexArray mVAO;
	VertexBufferOrig mVBO;
	VertexBufferLayout mVertexBufferLayout;
	IndexBuffer mEBO;
	Renderer mRenderer;

	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndexes;
	std::vector<Texture> mTextures;
};

