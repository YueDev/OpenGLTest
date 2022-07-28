#version 330 core

in vec4 vertexColor;
in vec2 texPos;

uniform sampler2D imageTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(imageTexture, texPos) * vec4(vertexColor);
}