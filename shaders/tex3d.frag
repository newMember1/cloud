#version 330 core

uniform sampler3D tex3D;
in vec3 OriPos;

out vec4 gColor;

void main()
{
    vec4 texColor = texture(tex3D, OriPos);
    if(texColor.a < 0.1)
        discard;
    gColor = texColor;
}