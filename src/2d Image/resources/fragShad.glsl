#version 410 core

in vec3 vertexColors;

out vec4 colors_;

void main()
{
	colors_ = vec4(vertexColors.x, vertexColors.y, vertexColors.z, 1.0f);
}