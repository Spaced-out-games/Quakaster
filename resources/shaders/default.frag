#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main() {
    FragColor = vec4(TexCoord.x,TexCoord.y,1.0f,1.0f);
}
