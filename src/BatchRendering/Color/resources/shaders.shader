#shader vertex	
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexturePosition;

out vec2 vTexCoord;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * position;
	vTexCoord = TexturePosition;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, vTexCoord);
};