#shader vertex	
#version 410 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colors;
layout(location = 3) in vec2 tex;

out vec3 outPositions;
out vec3 outNormal;
out vec3 outColors;
out vec2 outTex;

uniform mat4 uViewMatrix;
uniform mat4 uTranslation;
uniform mat4 uScale;
uniform mat4 uRotation;
uniform mat4 uModel;

void main()
{
	outPositions = vec3(uModel * uTranslation * -uRotation * uScale * vec4(positions, 1.0));
	outNormal = normal;
	outColors = colors;
	outTex = mat2(0.0, -1.0, 1.0, 0.0) * tex;
	gl_Position = uViewMatrix * vec4(positions, 1.0);
};

#shader fragment
#version 410 core

out vec4 fragColor;

in vec3 outPositions;
in vec3 outNormal;
in vec3 outColors;
in vec2 outTex;

uniform	sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camView;

vec4 pointLight()
{
    vec3 lightVec = lightPos - outPositions;

    float dist = length(lightVec);
    float a = 3.0;
    float b = 0.7;
    float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

    float ambient = 0.2f;

    vec3 normal = normalize(outNormal);
    vec3 lightDirection = normalize(lightVec);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0f); 

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camView - outPositions);
    vec3 reflectionDirection = reflect(-lightDirection, outNormal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;
    
    return (texture(diffuse0, outTex) * (diffuseStrength * intensity * ambient) + texture(specular0,outTex).r * specular * intensity) * lightColor;
}

vec4 directLight()
{
    float ambientLight = 0.20f;

    vec3 normalVec = normalize(outNormal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuseStrength = max(dot(normalVec, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camView - outPositions);
    vec3 reflectionDirection = reflect(-lightDirection, outNormal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, outTex) * (diffuseStrength * ambientLight) + texture(specular0,outTex).r * specular) * lightColor;
}
void main()
{
	fragColor = pointLight();
};