#version 330 core

uniform sampler3D tex3D;
in vec3 OriPos;

out vec4 gColor;

void main()
{
    gColor = texture(tex3D, OriPos);
    // gColor = vec4(0.2, 0.3, 0.5, 1.0);
}