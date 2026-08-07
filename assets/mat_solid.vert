#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in mat4 instance_transform;

layout(std140, binding = 0) uniform CameraData
{
    vec3 aTint;
};

out vec2 uv;
out vec3 tint;

void main() {
    gl_Position = instance_transform * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
    uv = aPos.xy;
    tint = aTint;
}
