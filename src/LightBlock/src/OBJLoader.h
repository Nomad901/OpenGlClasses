#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"

#include "VBO.h"

static std::vector<Vertex> loadOBJ(const std::filesystem::path& pPath)
{
	std::vector<glm::fvec3> vertexPositions;
	std::vector<glm::fvec3> vertexNormals;
	std::vector<glm::fvec2> vertexTexPos;
	
	std::vector<uint32_t> vertexPositionsIndices;
	std::vector<uint32_t> vertexNormalsIndices;
	std::vector<uint32_t> vertexTexPosIndices;

	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream ifstream(pPath);
	std::string line = "";

	if (!ifstream.is_open())
	{
		std::cout << std::format("Ifstream is not opened! File path: {}\n", pPath.string());
		return {};
	}


	return vertices;
}