#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shines;
	sampler2D textures[8];
	int index;
	int isJustColored;
};

struct pointLight
{
	vec3 position;
	vec3 direction;

	vec3 color;
	float intensity;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragPosTex;
in vec3 fragNormals;
in vec3 lightPos;

out vec4 finalColor;

uniform Material material;
uniform pointLight point_light[3];
uniform int uNumberOfLights;
uniform vec3 cameraPos;
uniform float uTime;

uniform vec3 uObjectColor;

void main()
{
	vec3 finalResult = vec3(0.0f);

	for(int i = 0; i < uNumberOfLights; ++i)
	{
		vec3 ambient = vec3(0.0f);
		if(material.isJustColored == 1)
		{
			ambient = point_light[i].color * point_light[i].ambient * material.ambient * point_light[i].intensity;
		}
		else 
		{
			ambient = point_light[i].color * point_light[i].ambient * material.ambient * 
					  point_light[i].intensity * vec3(texture(material.textures[0], fragPosTex));
		}

		vec3 norm = normalize(fragNormals);
		vec3 lightDir = normalize(point_light[i].position - fragPos);
		//vec3 lightDir = normalize(-point_light[i].direction);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = vec3(0.0f);
		if(material.isJustColored == 1)
		{
			diffuse = point_light[i].color * point_light[i].diffuse * (diff * material.diffuse) * point_light[i].intensity;
		}
		else
		{
			diffuse = point_light[i].color * point_light[i].diffuse * (diff * material.diffuse) * 
					  point_light[i].intensity * vec3(texture(material.textures[0], fragPosTex));
		}

		vec3 viewDir = normalize(cameraPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shines);
		vec3 specularMap = vec3(texture(material.textures[1], fragPosTex));
		vec3 specular = vec3(0.0f);
		if(material.isJustColored == 1)
		{	
			specular = point_light[i].color * point_light[i].specular * (spec * material.specular) * point_light[i].intensity;
		}
		else
		{
			specular = point_light[i].color * point_light[i].specular * (spec * material.specular) * 
					   point_light[i].intensity * specularMap;
		}

		vec3 emission = vec3(0.0f);
		if(texture(material.textures[1], fragPosTex).r == 0.0f)
		{
			emission = vec3(texture(material.textures[2], fragPosTex + vec2(0.0f, uTime * 0.75f)));
			emission = emission * (sin(uTime) * 0.5f + 0.5f) * 2.0f;
		}

		finalResult += ambient + diffuse + specular + emission;
	}

	vec4 result = vec4(0.0f);
	result = vec4(finalResult * uObjectColor, 1.0f);
	finalColor = result;
}