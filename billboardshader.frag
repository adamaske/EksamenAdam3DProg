#version 410 core
in vec2 UV;
uniform sampler2D textureSampler;
out vec4 fragmentColor;

void main(void)
{
    fragmentColor = texture2D(textureSampler, UV).rgba;
}
