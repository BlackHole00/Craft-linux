#version 330 core
out vec4 FragColor;

uniform sampler2D uTexture;

in vec2 oUv;

void main()
{
    FragColor = texture(uTexture, oUv);
}