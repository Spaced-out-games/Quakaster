#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
// uniform vec3 u_vector;
flat out vec3 local_position;

void main() {

    local_position = aPos;
    gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);

}