#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // The texture coordinates passed from the vertex shader

uniform sampler2D texture_0; // Sampler for the texture bound to GL_TEXTURE0

void main() {
    // Sample the texture at the provided texture coordinates
    FragColor = texture(texture_0, mod(TexCoord / 1, 1.0f)); // vec4(1.0,1.0,1.0,1.0);
}
