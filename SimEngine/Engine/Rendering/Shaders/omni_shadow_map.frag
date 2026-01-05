#version 460 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() 
{
    float depth = length(FragPos.xyz - lightPos) / farPlane;
    gl_FragDepth = depth;
}