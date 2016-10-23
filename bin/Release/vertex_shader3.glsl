#version 330 core

layout (location=1) in vec4 vPosition;

out VS_OUT
{
    vec4 color;
} vs_out;

uniform mat4 ViewProjection_Matrix;
uniform mat4 ModelPosition_Matrix;

void main(void)
{
    gl_Position = ViewProjection_Matrix*ModelPosition_Matrix*vPosition;
    vs_out.color = vPosition*2.0+vec4(0.5,0.5,0.5,0.0);
}
