#version 430 core
out vec4 FragColor;

// test
layout(binding = 0) uniform sampler2D diffuse;
in vec2 uv;
in vec3 tint;


void main()
{
    // ensure we at least get a result
    FragColor = texture(diffuse, uv) + vec4(tint, 1.0);
}
