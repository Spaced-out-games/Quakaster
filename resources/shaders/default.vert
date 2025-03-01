#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;
uniform vec3 u_scale = vec3(1.0f,1.0f,5.0f);

out vec2 TexCoord;

void main() {
    gl_Position = u_proj * u_view * u_model *  vec4(u_scale * aPos, 1.0);
    TexCoord = aPos.xz / 10;
}