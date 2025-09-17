#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shines;
	sampler2D textures[8];
	int diffuseIndex;
	int specularIndex;
	int isJustColored;
};

struct Light
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 color;
	float intensity;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 minBounds;
	vec3 maxBounds;
	int useBounds;
};

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragPosTex;
in vec3 fragNormals;
in vec3 lightPos;

out vec4 finalColor;

uniform Material material;
uniform Light light[16];
uniform int uNumberOfLights;
uniform vec3 cameraPos;

uniform vec3 uObjectColor;

float near = 0.1f;
float far = 150.0f;

float linearizeDepth(float pDepth)
{
	float z = pDepth * 2.0f - 1.0f;
	return (2.0f * near * far) / (far + near - z * (far - near));
}

void main()
{
	vec3 finalResult = vec3(0.0f);

	for(int i = 0; i < uNumberOfLights; ++i)
	{
		// ambient
		vec3 ambient = vec3(0.0f);
		if(material.isJustColored == 1)
		{
			ambient = light[i].color * light[i].ambient * material.ambient * light[i].intensity;
		} 
		else 
		{
			ambient = light[i].color * light[i].ambient * material.ambient * 
					  light[i].intensity * vec3(texture(material.textures[material.diffuseIndex], fragPosTex));
		}
		
		// difuse 
		vec3 norm = normalize(fragNormals);
		vec3 lightDir = normalize(light[i].position - fragPos);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = vec3(0.0f);
		if(material.isJustColored == 1)
		{
			diffuse = light[i].color * light[i].diffuse * (diff * material.diffuse) * light[i].intensity;
		}
		else
		{
			diffuse = light[i].color * light[i].diffuse * (diff * material.diffuse) * 
					  light[i].intensity * vec3(texture(material.textures[material.diffuseIndex], fragPosTex));
		}

		// specular
		vec3 viewDir = normalize(cameraPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shines);
		vec3 specular = vec3(0.0f);
		if(material.isJustColored == 1)
		{	
			specular = light[i].color * light[i].specular * (spec * material.specular) * light[i].intensity;
		}
		else
		{
			specular = light[i].color * light[i].specular * (spec * material.specular) * 
					   light[i].intensity * vec3(texture(material.textures[material.specularIndex], fragPosTex));
		}
		
		// attenuation
		//float distance = length(light[i].position - fragPos);
		//float attenuation = 1.0f / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
		//
		//ambient  *= attenuation;
		//diffuse  *= attenuation;
		//specular *= attenuation;
		 
		finalResult += ambient + diffuse + specular;
	}

	vec4 result = vec4(0.0f);
	float depth = linearizeDepth(gl_FragCoord.z) / far;
	vec4 depthVec = vec4(vec3(pow(depth, 1.4f)), 1.0f);
	result = vec4(finalResult * uObjectColor, 1.0f) * (1 - depthVec) + depthVec;
	finalColor = result;
	//finalColor = vec4(vec3(depth), 1.0f);
}
