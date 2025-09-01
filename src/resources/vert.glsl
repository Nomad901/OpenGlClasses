#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors; // location - is looking for VAO or VBO, which both of them have index of 1 and 2

uniform float uOffsetY; // just a global variable 
uniform float uOffsetX;

out vec3 v_vertexColors;

void main()
{
	v_vertexColors = vertexColors;

	gl_Position = vec4(position.x + uOffsetX, position.y + uOffsetY, position.z, 1.0f);
}