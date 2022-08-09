#version 330 core

in vec2 texPos;

uniform sampler2D imageTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(imageTexture, texPos);
}