#version 460 core

layout (location = 0) in vec3 pos;

layout (location = 3) in mat4 instanceMatrix;

uniform bool useInstanceRendering;

uniform mat4 model;
uniform mat4 directionalLightProjection;

void main() 
{
    mat4 modelMatrix = useInstanceRendering ? instanceMatrix : model;
    
    gl_Position = directionalLightProjection * modelMatrix * vec4(pos, 1.0);
}
