#include "Model.h"

Model::Model(const std::filesystem::path& pPath)
{
	load(pPath);
}

void Model::load(const std::filesystem::path& pPath)
{
	loadModel(pPath);
}

void Model::draw(std::string_view pNameUniform, Shader& pShader)
{
	for (auto& i : mMeshes)
	{
		//i.draw(pNameUniform, pShader);
		i.drawForModels(pShader);
	}
}

void Model::loadModel(const std::filesystem::path& pPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << std::format("ERROR IN ASSIMP: {}\n", importer.GetErrorString());
		return;
	}
	mDirectory = pPath.string().substr(0, pPath.string().find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* pNode, const aiScene* pScene)
{
	for (uint32_t i = 0; i < pNode->mNumMeshes; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		if (!mesh)
			std::cout << "Mesh is wrong in process node!\n";
	 	processMesh(mesh, pScene);
	}
	for (uint32_t i = 0; i < pNode->mNumChildren; ++i)
	{
		processNode(pNode->mChildren[i], pScene);
	}
}

void Model::processMesh(aiMesh* pMesh, const aiScene* pScene)
{
	std::vector<Vertex> vertices;
	vertices.reserve(100);
	std::vector<uint32_t> indices; 
	indices.resize(100);
	std::vector<Texture2> textures;
	textures.reserve(100);

	for (uint32_t i = 0; i < pMesh->mNumVertices; ++i)
	{
		Vertex vertex;
		glm::vec3 pos;
		glm::vec3 normals;
		glm::vec4 colors;
		float texInd = 0.0f;

		pos.x = pMesh->mVertices[i].x;
		pos.y = pMesh->mVertices[i].y;
		pos.z = pMesh->mVertices[i].z;

		normals.x = pMesh->mNormals[i].x;
		normals.y = pMesh->mNormals[i].y;
		normals.z = pMesh->mNormals[i].z;

		colors.x = 0.0f;
		colors.y = 0.0f;
		colors.z = 0.0f;
		colors.w = 0.0f;

		if (pMesh->mTextureCoords[0])
		{
			glm::vec2 texPos;
			texPos.x = pMesh->mTextureCoords[0][i].x;
			texPos.y = pMesh->mTextureCoords[0][i].y;
			vertex.mTexCoord = texPos;
		}
		else
			vertex.mTexCoord = { 0.0f, 0.0f };

		vertex.mPos = pos;
		vertex.mNormals = normals;
		vertex.mColor = colors;
		 
		vertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < pMesh->mNumFaces; ++i)
	{
		aiFace face = pMesh->mFaces[i];
		for (uint32_t i = 0; i < face.mNumIndices; ++i)
		{
			indices.push_back(face.mIndices[i]);
		}
	}
	if (pMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
		std::vector<Texture2> diffuseMaterial;
		loadMaterialTextures(diffuseMaterial, material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaterial.begin(), diffuseMaterial.end());
		std::vector<Texture2> specularMaterial;
		loadMaterialTextures(specularMaterial, material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaterial.begin(), specularMaterial.end());
		//std::vector<Texture> normalMaterial;
		//loadMaterialTextures(normalMaterial, material, aiTextureType_AMBIENT, "texture_normal");
		//textures.insert(textures.end(), normalMaterial.begin(), normalMaterial.end());
		//std::vector<Texture> heightMaterial;
		//loadMaterialTextures(heightMaterial, material, aiTextureType_HEIGHT, "texture_height");
		//textures.insert(textures.end(), heightMaterial.begin(), heightMaterial.end());
	}

	mMeshes.emplace_back(vertices, indices, std::move(textures));
}

void Model::loadMaterialTextures(std::vector<Texture2>& pStrgToChange, aiMaterial* pMaterial,
								 aiTextureType pTexType, std::string pTypeName)
{
	for (uint32_t i = 0; i < pMaterial->GetTextureCount(pTexType); ++i)
	{
		aiString string;
		pMaterial->GetTexture(pTexType, i, &string);
		bool skip = false;
		for (uint32_t j = 0; j < mTexturesLoaded.size(); ++j)
		{
			if (std::strcmp(mTexturesLoaded[j].getPath().string().data(), string.C_Str()) == 0)
			{
				pStrgToChange.emplace_back(mTexturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture2 texture;
			texture.setID(textureFromFile(string.C_Str(), mDirectory, false));
			texture.setType(pTypeName);
			texture.setPath(string.C_Str());
			pStrgToChange.push_back(texture);
			mTexturesLoaded.push_back(texture);
		}
	}
}

uint32_t Model::textureFromFile(std::string_view pFileName, 
								const std::filesystem::path& pPath, 
								bool gamma)
{
	std::string filename;
	std::filesystem::path fileNamePath = pFileName;
	
	if (fileNamePath.is_absolute())
		filename = pFileName;
	else
		filename = pPath.string() + '/' + std::string(pFileName);

	uint32_t id;
	glGenTextures(1, &id);

	stbi_set_flip_vertically_on_load(true);

	int32_t width, height, BPP;
	uint8_t* data = stbi_load(filename.c_str(), &width, &height, &BPP, 0);
	if (!data)
	{
		std::cout << "Couldnt load the image-texture!\n";
		stbi_image_free(data);
		return 0;
	}

	GLenum format{};
	if (BPP == 1)
		format = GL_RED;
	else if (BPP == 3)
		format = GL_RGB;
	else if (BPP == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, id);	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (data)
		stbi_image_free(data);
	
	return id;
}
	