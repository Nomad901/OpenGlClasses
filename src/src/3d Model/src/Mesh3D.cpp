#include "Mesh3D.h"

Mesh3D::Mesh3D(const std::vector<Vertex>& pVertexes, 
			   const std::vector<GLuint>& pIndicies,
			   const std::vector<Texture>& pTextures)
	: mTextures(pTextures), mVertexes(pVertexes), 
	  mIndicies(pIndicies)
{
	mVAO.bind();
	VertexBuffer VBO(pVertexes);
	IndexBuffer IBO(pIndicies);
	VertexBufferLayout VBL;

	VBL.push(GL_FLOAT, 3);
	VBL.push(GL_FLOAT, 3);
	VBL.push(GL_FLOAT, 3);
	VBL.push(GL_FLOAT, 2);

	mVAO.addBuffer(VBO, VBL);

	mVAO.unbind();
	VBO.unbind();
	IBO.unbind();
}

void Mesh3D::draw(Shader& pShader, 
				  Camera& pCamera, 
				  const glm::mat4& pMatrix, 
				  const glm::quat& pRotate, 
				  const glm::vec3& pTranslate,
				  const glm::vec3& pScale)
{
	pShader.bind();
	mVAO.bind();

	uint32_t numDiffuse = 0;
	uint32_t numSpecular = 0;

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		std::string num;
		std::string type = mTextures[i].getType();
		if (type == "diffuse")
			num = std::to_string(numDiffuse++);
		else if (type == "specular")
			num = std::to_string(numSpecular++);
		mTextures[i].texSlot(pShader, (type + num), i);
		mTextures[i].bind();
	}
	pShader.setUniform3f("uViewMatrix", pCamera.getPosition().x, pCamera.getPosition().y, pCamera.getPosition().z);
	pCamera.matrix(pShader, "uViewMatrix");

	glm::mat4 rotate    = glm::mat4(1.0f);
	glm::mat4 scale     = glm::mat4(1.0f);
	glm::mat4 translate = glm::mat4(1.0f);

	rotate = glm::mat4_cast(pRotate);
	scale = glm::scale(pScale);
	translate = glm::translate(pTranslate);

	pShader.setUniform4fv("uModel", pMatrix);
	pShader.setUniform4fv("uTranslation", translate);
	pShader.setUniform4fv("uScale", scale);
	pShader.setUniform4fv("uRotation", rotate);

	GLCall(glDrawElements(GL_TRIANGLES, mIndicies.size(), GL_UNSIGNED_INT, 0));
}
