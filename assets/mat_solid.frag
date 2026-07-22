#version 430 core
out vec4 FragColor;

// test
layout(binding = 0) uniform sampler2D diffuse;
in vec2 uv;


void main()
{
    // ensure we at least get a result
    FragColor = vec4(1.0, 0.2, 0.3, 1.0)+ texture(diffuse, uv);
}
