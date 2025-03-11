#version 330 core
out vec4 FragColor;

flat in vec3 local_position;

void main() {
    // Sample the texture at the provided texture coordinates
    FragColor = vec4(local_position,1.0);
}