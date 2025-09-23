// geom

#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

in VS_OUT
{
	vec3 normal;
} vs_in[];

const float magnitude = 1.0f;

uniform mat4 uProj;

void generateLine(int pIndex)
{
	gl_Position = uProj * gl_in[pIndex].gl_Position;
	EmitVertex();
	gl_Position = uProj * (gl_in[pIndex].gl_Position + vec4(vs_in[pIndex].normal, 0.0f) * magnitude);
	EmitVertex();
	EndPrimitive();
}

void generateFaceNormals()
{
	vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0f;

	vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
	vec3 faceNormal = normalize(cross(a, b));

	gl_Position = uProj * center;
	EmitVertex();
	gl_Position = uProj * (center + vec4(faceNormal, 0.0f) * magnitude);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	generateFaceNormals();
}


// vert#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out VS_OUT
{
	vec3 normal;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;

void main()
{
	gl_Position = uView * uModel * vec4(pos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(uView * uModel)));
	vs_out.normal = normalize(vec3(vec4(normalMatrix * normals, 0.0f)));
}


// frag
#version 430 core

out vec4 fragColor;

uniform vec3 uColor;

void main()
{
	fragColor = vec4(uColor, 1.0f);
}


