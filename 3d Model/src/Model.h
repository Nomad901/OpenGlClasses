#pragma once
#include <filesystem>

#include "JSON/json.hpp"
#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh3D.h"

class Renderer;

class Model
{
public:
	Model(const std::filesystem::path& pPath);

	void draw(Shader& pShader, Camera& pCamera);
	
private:
	std::vector<uint32_t> getData();
	std::vector<float> getFloats(const nlohmann::json& pAccessor);
	std::vector<GLuint> getIndicies(const nlohmann::json& pAccessor);

	std::vector<Vertex> assembleVertexes(const std::vector<glm::vec3>& pPositions,
									     const std::vector<glm::vec3>& pNormals,
									     const std::vector<glm::vec2>& pTexUV);

	std::vector<Texture> getTextures();

	std::vector<glm::vec2> groupFloatsVec2(const std::vector<float>& pFloatsVec);
	std::vector<glm::vec3> groupFloatsVec3(const std::vector<float>& pFloatsVec);
	std::vector<glm::vec4> groupFloatsVec4(const std::vector<float>& pFloatsVec);

	void loadMeshes(uint32_t pIndexMesh);
	void traverseNode(uint32_t pNextNode, glm::mat4 pMatrix = glm::mat4(1.0f));

private:
	std::vector<uint32_t> mData;
	std::vector<std::string> mLoadedTexName;
	std::vector<Texture> mLoadedTex;
	std::vector<Mesh3D> mMeshes;
	std::vector<glm::mat4> mMatricesMesh;
	std::vector<glm::vec3> mTranslationMesh;
	std::vector<glm::vec3> mScaleMesh;
	std::vector<glm::quat> mRotateMesh;

	std::filesystem::path mPath;
	nlohmann::json mJson;

};

