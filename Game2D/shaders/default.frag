#version 330 core
in vec4 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool uUsingTexture = false;

void main()
{
	vec4 texColor = texture(uTexture, TexCoord);
	if (uUsingTexture)
		FragColor = texColor * vertexColor;
	else
		FragColor = vertexColor;
}