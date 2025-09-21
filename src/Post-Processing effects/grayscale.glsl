#version 440

in vec2 fragPosTex;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	//vec3 color = texture(uTexture, fragPosTex).rgb;
	//outColor = vec4(color, 1.0f);

	// first variant
	//
	// outColor = texture(uTexture, fragPosTex);
	// float average = (outColor.r + outColor.g + outColor.b) / 3.0f;
	// outColor = vec4(average, average, average, 1.0f);


	// second variant

	outColor = texture(uTexture, fragPosTex);
	float average = 0.2126 * outColor.r + 0.7152 * outColor.g + 0.0722 * outColor.b;
	outColor = vec4(average, average, average, 1.0f);
}
