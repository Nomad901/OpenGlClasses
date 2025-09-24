Sphere::Sphere(const glm::vec4& pColor, float pRadius, int32_t pSectors, int32_t pStacks)
{
	std::vector<Vertex> vertices;
	vertices.reserve((pSectors + 1) * (pStacks + 1));
	std::vector<uint32_t> indices;
	indices.reserve((pSectors + 1) * (pStacks + 1) * 6);
	
	for (int32_t i = 0; i <= pStacks; ++i)
	{
		float stackAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / pStacks);
		float xy = pRadius * cosf(stackAngle);
		float z = pRadius * sinf(stackAngle);

		for (int32_t j = 0; j <= pSectors; ++j)
		{
			float sectorAngle = j * (2 * glm::pi<float>() / pSectors);
			
			Vertex vertex;
			vertex.mPos.x = xy * cosf(sectorAngle);
			vertex.mPos.y = xy * sinf(sectorAngle);
			vertex.mPos.z = z;

			vertex.mNormals = glm::normalize(vertex.mPos);

			vertex.mTexCoord.x = (float)j / pSectors;
			vertex.mTexCoord.y = (float)i / pStacks;

			vertex.mColor = pColor;

			vertices.push_back(vertex);
		}
	}

	for (int32_t i = 0; i < pStacks; ++i)
	{
		for (int32_t j = 0; j < pSectors; ++j)
		{
			int32_t first  = (i * (pSectors + 1)) + j;
			int32_t second = first + pSectors + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);
			indices.push_back(first + 1);
			indices.push_back(second);
			indices.push_back(second + 1);
		}
	}

	setVertexStrg(vertices);
	setIndexStrg(indices);
}
