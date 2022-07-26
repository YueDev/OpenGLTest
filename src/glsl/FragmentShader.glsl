#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

uniform float mixf;
uniform sampler2D hanaTexture;
uniform sampler2D stickerTexture;

out vec4 outColor;

void main() {
    outColor = mix(texture(hanaTexture, texCoord), texture(stickerTexture, texCoord), mixf);
}