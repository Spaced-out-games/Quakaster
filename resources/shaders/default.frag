#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec3 u_vertexcolor = vec3(1.0,1.0,1.0);

//uniform sampler2D screenTexture;

void main() {
    FragColor = vec4(u_vertexcolor, 1.0f);
}
