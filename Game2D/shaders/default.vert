#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uTransform;

out vec4 vertexColor;
out vec2 TexCoord;

void main() {
	vertexColor = aColor;
	gl_Position = uTransform * vec4(aPos.x, aPos.y, 0.0, 1.0);
	TexCoord = aTexCoords;
}