#include "Material.h"
#include "Texture2.h"

Material::Material(glm::vec3 pAmbient, 
				   glm::vec3 pDiffuse, 
				   glm::vec3 pSpecular, 
				   std::vector<Texture2>& pTextures)
{
	init(pAmbient, pDiffuse, pSpecular, pTextures);
}

void Material::init(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular, std::vector<Texture2>& pTextures)
{
	mAmbient = pAmbient;
	mDiffuse = pDiffuse;
	mSpecular = pSpecular;
	mTextures.reserve(pTextures.size());
	mTextures = pTextures;
}

void Material::sendToShader(Shader& pShader, bool isJustColored)
{
	pShader.setUniform3fv("material.ambient", mAmbient);
	pShader.setUniform3fv("material.diffuse", mDiffuse);
	pShader.setUniform3fv("material.specular", mSpecular);
	for (int32_t i = 0; i < std::ssize(mTextures) && i < 8; ++i)
	{
		std::string unifString = mTextures[i].getUniformName() + '[' + std::to_string(i) + ']';
		pShader.setUnifrom1i(unifString, i);
	}
	pShader.setUnifrom1i("material.index", 0);
	if (isJustColored)
		pShader.setUnifrom1i("material.isJustColored", 1);
	else 
		pShader.setUnifrom1i("material.isJustColored", 0);
}
