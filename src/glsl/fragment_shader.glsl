#version 330 core

in vec2 texPos;

uniform sampler2D imageTexture1;
uniform sampler2D imageTexture2;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(imageTexture1, texPos), texture(imageTexture2, texPos), 0.5f);
}