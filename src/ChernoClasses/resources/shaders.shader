#shader vertex	
#version 410 core

layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 TexturePosition;
layout(location = 1) in vec4 secondPos;

//out vec2 vTexCoord;

void main()
{
	gl_Position = position;
	//gl_Position = secondPos;
	//vTexCoord = TexturePosition;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 color2;
//in vec2 vTexCoord;

uniform vec4 uColorSec;
uniform vec4 uColor;
//uniform sampler2D uTexture;

void main()
{
	//vec4 texColor = texture(uTexture, vTexCoord);
	//color = texColor;
	color = uColor;
	color = uColorSec;
};