#version 330 core

out vec4 FragColor;

uniform sampler2D uTex;

in vec2 aUV;

void main()
{
    FragColor = texture(uTex, vec2(aUV.x, -aUV.y));
    /*FragColor = vec4(
        1.0,
        0.2,
        0.3,
        1.0);*/
}
