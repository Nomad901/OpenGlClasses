#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D matTexture[8];
	int index;
	int isJustColored;
};

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragPosTex;
in vec3 fragNormals;

out vec4 finalColor;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

vec3 getDiffuseLight(Material material, vec3 fragPos, vec3 fragNormals, vec3 lightPos0)
{
	vec3 fragToLightDir = normalize(lightPos0 - fragPos);
	vec3 normal = normalize(fragNormals);
	float diffuseIntensity = max(dot(fragToLightDir, normal), 0.0);
	vec3 diffuseFinal = material.diffuse * diffuseIntensity;
	return diffuseFinal;
}

vec3 getSpecularLight(Material material, vec3 lightPos0, vec3 fragPos, vec3 fragNormals, vec3 cameraPos)
{
	vec3 fragToLightDir = normalize(lightPos0 - fragPos);
	vec3 fragToViewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-fragToLightDir, normalize(fragNormals));
	float specularIntensity = pow(max(dot(fragToViewDir, reflectDir), 0.0), 60);
	vec3 specularFinal = material.specular * specularIntensity * texture(material.matTexture[1], fragPosTex).rgb; 
	return specularFinal;
}

void main()
{
	vec3 ambientLight = material.ambient;
	vec3 diffuseLight = getDiffuseLight(material, fragPos, fragNormals, lightPos0);
	vec3 specularFinal = getSpecularLight(material, lightPos0, fragPos, fragNormals, cameraPos);

	vec4 textureColor;

	if(material.isJustColored == 0)
	{
		switch(material.index)
		{
		case 0: textureColor = texture(material.matTexture[0], fragPosTex); break;
		case 1: textureColor = texture(material.matTexture[1], fragPosTex); break;
		case 2: textureColor = texture(material.matTexture[2], fragPosTex); break;
		case 3: textureColor = texture(material.matTexture[3], fragPosTex); break;
		case 4: textureColor = texture(material.matTexture[4], fragPosTex); break;
		case 5: textureColor = texture(material.matTexture[5], fragPosTex); break;
		case 6: textureColor = texture(material.matTexture[6], fragPosTex); break;
		case 7: textureColor = texture(material.matTexture[7], fragPosTex); break;
		default: textureColor = vec4(1.0f); break;
		}
		finalColor = textureColor * fragColor * 
					 vec4(ambientLight + diffuseLight + specularFinal, 1.0f);
	}
	else
	{
		finalColor = fragColor;  
	}
}