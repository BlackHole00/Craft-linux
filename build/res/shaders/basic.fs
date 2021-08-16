#version 330 core
out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uAlpha = 1.0f;

in vec2 oUv;

void main()
{
    vec4 textureColor = texture(uTexture, oUv);
    FragColor = vec4(textureColor.x, textureColor.y, textureColor.z, uAlpha);
}