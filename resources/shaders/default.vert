#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;


out vec2 TexCoord;

void main() {
    gl_Position = u_proj * u_view * vec4(aPos, 1.0);
    TexCoord = aPos.xz;
}