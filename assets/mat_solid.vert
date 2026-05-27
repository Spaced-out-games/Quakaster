#version 330 core

layout(location = 0) in vec3 aPos;

layout(std140) uniform CameraData {
	mat4 view;
	mat4 proj;

};

void main()
{
    gl_Position = view * vec4(aPos, 1.0);
}
