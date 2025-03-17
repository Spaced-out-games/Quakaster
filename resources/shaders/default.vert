#version 330 core

uniform mat4 u_view;
uniform mat4 u_proj;
layout(location = 0) in vec4 instanceTransform1; // First column of the instance matrix
layout(location = 1) in vec4 instanceTransform2; // Second column of the instance matrix
layout(location = 2) in vec4 instanceTransform3; // Third column of the instance matrix
layout(location = 3) in vec4 instanceTransform4; // Fourth column of the instance matrix
layout(location = 4) in vec3 aPos;
out vec2 TexCoord;

void main() {
    mat4 instanceMatrix = mat4(instanceTransform1, instanceTransform2, instanceTransform3, instanceTransform4);

    vec4 worldPos = instanceMatrix * vec4(aPos, 1.0);
    gl_Position = u_proj * u_view * worldPos;

    TexCoord = worldPos.xz;
}
