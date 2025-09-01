#include "Model.h"

std::string get_file_contents(const std::filesystem::path& filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Model::Model(const std::filesystem::path& pPath)
{
	std::string text = get_file_contents(pPath);
	mJson = nlohmann::json::parse(text);

	mPath = pPath;
	mData = getData();

	traverseNode(0);
}

std::vector<uint32_t> Model::getData()
{
	std::string bytesText;
	std::string uri = mJson["buffers"][0]["uri"];

	std::string fileStr = mPath.string();
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents(fileDirectory + uri);

	std::vector<uint32_t> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::getFloats(const nlohmann::json& pAccessor)
{
	std::vector<float> floatsVec;

	uint32_t bufferViewIndex = pAccessor.value("bufferView", 1); // retrieves value from the key. if key wasnt found - will return 1;
	uint32_t count = pAccessor["count"];
	uint32_t accByteOffset = pAccessor.value("byteOffset", 0); // the same, but instead of 1 will return 0;
	std::string type = pAccessor["type"];

	nlohmann::json bufferView = mJson["bufferViews"][bufferViewIndex];
	uint32_t byteOffset = bufferView["byteOffset"]; 

	uint32_t numPerVert;
	if (type == "SCALAR")
		numPerVert = 1;
	else if (type == "VEC2")
		numPerVert = 2;
	else if (type == "VEC3")
		numPerVert = 3;
	else if (type == "VEC4")
		numPerVert = 4;
	else
		throw std::invalid_argument("Type of the data is invalid! Not scalar, vec2, vec3 and vec4\n");

	uint32_t beginningOfData = byteOffset + accByteOffset;
	uint32_t lengthData = count * 4 * numPerVert;
	for (uint32_t i = beginningOfData; i < beginningOfData + lengthData; )
	{
		uint8_t bytes[] = { mData[i++], mData[i++], mData[i++], mData[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatsVec.push_back(value);
	}

	return floatsVec;
}

std::vector<GLuint> Model::getIndicies(const nlohmann::json& pAccessor)
{
	std::vector<GLuint> indicies;

	uint32_t bufferViewIndex = pAccessor.value("bufferView", 0);
	uint32_t count = pAccessor["count"];
	uint32_t accByteOffset = pAccessor.value("byteOffset", 0);
	uint32_t type = pAccessor["componentType"];

	nlohmann::json bufferView = mJson["bufferViews"][bufferViewIndex];
	uint32_t byteOffset = bufferView["byteOffset"];

	uint32_t beginningOfData = byteOffset + accByteOffset;
	if (type == 5125)
	{
		for (uint32_t i = beginningOfData; i < byteOffset + accByteOffset + count * 4; )
		{
			uint8_t bytes[] = { mData[i++], mData[i++], mData[i++], mData[i++] };
			uint32_t value;
			std::memcpy(&value, bytes, sizeof(uint32_t));
			indicies.push_back((GLuint)value);
		}
	}
	else if (type == 5123)
	{
		for (uint32_t i = beginningOfData; i < byteOffset + accByteOffset + count * 2; )
		{
			uint8_t bytes[] = { mData[i++], mData[i++], mData[i++], mData[i++] };
			uint16_t value;
			std::memcpy(&value, bytes, sizeof(uint16_t));
			indicies.push_back((GLuint)value);
		}
	}
	else if (type == 5122)
	{
		for (uint32_t i = 0; i < byteOffset + accByteOffset + count; )
		{
			uint8_t bytes[] = { mData[i++], mData[i++], mData[i++], mData[i++] };
			int16_t value;
			std::memcpy(&value, bytes, sizeof(int16_t));
			indicies.push_back((GLuint)value);
		}
	}
	else
	{
		std::cout << "Type of GLuint Indicies is invalid!\n";
		return std::vector<GLuint>();
	}

	return indicies;
}

std::vector<Vertex> Model::assembleVertexes(const std::vector<glm::vec3>& pPositions,
										    const std::vector<glm::vec3>& pNormals,
										    const std::vector<glm::vec2>& pTexUV)
{
	std::vector<Vertex> vertexes;
	for (size_t i = 0; i < pPositions.size(); ++i)
	{
		vertexes.push_back(Vertex{ pPositions[i], 
								   pNormals[i],
								   glm::vec3(1.0f,1.0f,1.0f), 
								   pTexUV[i] });
	}
	return vertexes;
}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = mPath.string();
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	for (uint32_t i = 0; i < mJson["images"].size(); ++i)
	{
		std::string texPath = mJson["images"][i]["uri"];

		bool skip = false;
		for (size_t j = 0; j < mLoadedTexName.size(); ++j)
		{
			if (mLoadedTexName[i] == texPath)
			{
				textures.push_back(mLoadedTex[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory+ texPath), "diffuse", mLoadedTex.size());
				textures.push_back(diffuse);
				mLoadedTex.push_back(diffuse);
			}
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath), "specular", mLoadedTex.size());
				textures.push_back(specular);
				mLoadedTex.push_back(specular);
			}
			mLoadedTexName.push_back(texPath);
		}
	}
	
	return textures;
}

std::vector<glm::vec2> Model::groupFloatsVec2(const std::vector<float>& pFloatsVec)
{
	std::vector<glm::vec2> finalVector;
	for (size_t i = 0; i < pFloatsVec.size(); )
	{
		finalVector.push_back(glm::vec2(pFloatsVec[i++], pFloatsVec[i++]));
	}
	return finalVector;
}

std::vector<glm::vec3> Model::groupFloatsVec3(const std::vector<float>& pFloatsVec)
{
	std::vector<glm::vec3> finalVector;
	for (size_t i = 0; i < pFloatsVec.size(); )
	{
		finalVector.push_back(glm::vec3(pFloatsVec[i++], pFloatsVec[i++], pFloatsVec[i++]));
	}
	return finalVector;
}

std::vector<glm::vec4> Model::groupFloatsVec4(const std::vector<float>& pFloatsVec)
{
	std::vector<glm::vec4> finalVector;
	for (size_t i = 0; i < pFloatsVec.size(); )
	{
		finalVector.push_back(glm::vec4(pFloatsVec[i++], pFloatsVec[i++], pFloatsVec[i++], pFloatsVec[i++]));
	}
	return finalVector;
}

void Model::loadMeshes(uint32_t pIndexMesh)
{
	uint32_t posAccInd = mJson["meshes"][pIndexMesh]["primitives"][0]["attributes"]["POSITION"];
	uint32_t normalAccInd = mJson["meshes"][pIndexMesh]["primitives"][0]["attributes"]["NORMAL"];
	uint32_t texAccInd = mJson["meshes"][pIndexMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	uint32_t indiciesAccInd = mJson["meshes"][pIndexMesh]["primitives"][0]["indices"];
	
	std::vector<float> posVec = getFloats(mJson["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalsVec = getFloats(mJson["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalsVec);
	std::vector<float> texVec = getFloats(mJson["accessors"][texAccInd]);
	std::vector<glm::vec2> textUV = groupFloatsVec2(texVec);

	std::vector<Vertex> vertices = assembleVertexes(positions, normals, textUV);
	std::vector<GLuint> indicies = getIndicies(mJson["accessors"][indiciesAccInd]);
	std::vector<Texture> textures = getTextures();

	mMeshes.push_back(Mesh3D(vertices, indicies, textures));
}

void Model::traverseNode(uint32_t pNextNode, glm::mat4 pMatrix)
{
	nlohmann::json currentNode = mJson["nodes"][pNextNode];

	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (currentNode.find("translation") != currentNode.end())
	{
		float transValues[3];
		for (uint32_t i = 0; i < currentNode["translation"].size(); ++i)
		{
			transValues[i] = currentNode["translation"][i];
		}
		translation = glm::make_vec3(transValues);
	}

	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (currentNode.find("rotation") != currentNode.end())
	{
		float rotationValues[4] =
		{
			currentNode["rotation"][3],
			currentNode["rotation"][0],
			currentNode["rotation"][1],
			currentNode["rotation"][2]
		};
		rotation = glm::make_quat(rotationValues);
	}

	glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
	if (currentNode.find("scale") != currentNode.end())
	{
		float scaleValues[3];
		for (uint32_t i = 0; i < currentNode["scale"].size(); ++i)
		{
			scaleValues[i] = currentNode["scale"][i];
		}
		scale = glm::make_vec3(scaleValues);
	}

	glm::mat4 matrixNode = glm::mat4(1.0f);
	if (currentNode.find("matrix") != currentNode.end())
	{
		float matValues[16];
		for (uint32_t i = 0; i < currentNode["matrix"].size(); ++i)
		{
			matValues[i] = currentNode["matrix"][i];
		}
		matrixNode = glm::make_mat4(matValues);
	}

	glm::mat4 matTranslate = glm::mat4(1.0f);
	glm::mat4 matRotate = glm::mat4(1.0f);
	glm::mat4 matScale = glm::mat4(1.0f);

	matTranslate = glm::translate(translation);
	matRotate = glm::mat4_cast(rotation);
	matScale = glm::scale(scale);

	glm::mat4 matNextNode = pMatrix * matrixNode * matTranslate * matRotate * matScale;

	if (currentNode.find("mesh") != currentNode.end())
	{
		mTranslationMesh.push_back(translation);
		mScaleMesh.push_back(scale);
		mRotateMesh.push_back(rotation);
		mMatricesMesh.push_back(matrixNode);

		loadMeshes(currentNode["mesh"]);
	}

	if (currentNode.find("children") != currentNode.end())
	{
		for (uint32_t i = 0; i < currentNode["children"].size(); ++i)
		{
			traverseNode(currentNode["children"][i], matrixNode);
		}
	}
}

void Model::draw(Shader& pShader, Camera& pCamera)
{
	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].draw(pShader, pCamera, mMatricesMesh[i]);
	}
}