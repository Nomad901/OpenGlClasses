#shader vertex	
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 lColor;
layout(location = 2) in vec2 TexturePosition;
layout(location = 3) in float textureInd;

out vec2 vTexCoord;
out vec4 outColor;
flat out float texInd;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * position;
	vTexCoord = TexturePosition;
	outColor = lColor;
	texInd = textureInd;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;
in vec4 outColor;
flat in float texInd;

uniform sampler2D uTexture[2];

void main()
{
	int index = int(texInd);
	color = texture(uTexture[index], vTexCoord);
};