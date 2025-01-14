#version 330 core
out vec4 FragColor;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
uniform vec3 u_vector;


void main() {
    // Sample the texture at the provided texture coordinates
    FragColor = vec4(0.0,0.0,1.0,1.0);
}