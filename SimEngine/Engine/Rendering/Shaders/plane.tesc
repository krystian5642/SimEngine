#version 460

layout (vertices = 3) out;

uniform float tesselationLevel;

void main() 
{
    gl_TessLevelOuter[0] = tesselationLevel;
    gl_TessLevelOuter[1] = tesselationLevel;
    gl_TessLevelOuter[2] = tesselationLevel;
    
    gl_TessLevelInner[0] = tesselationLevel;
    
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}