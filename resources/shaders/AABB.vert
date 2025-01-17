#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_proj;
uniform vec3 u_location = vec3(1.0,1.0,1.0);
uniform vec3 u_dimensions = vec3(1.0,1.0,1.0);

void main() {
    gl_Position = u_proj * u_view * vec4((aPos * u_dimensions) + u_location, 1.0);

}
