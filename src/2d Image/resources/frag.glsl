#version 410 core

in vec3 v_vertexColors;
in vec2 objTextureFrag;

out vec4 color;

uniform sampler2D tex0;

void main()
{
    color = texture(tex0, objTextureFrag);
	//color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);
}