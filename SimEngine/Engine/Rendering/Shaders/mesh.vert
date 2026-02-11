#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionaLightPos;

uniform bool useInstanceRendering;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 directionalLightProjection;

void main()
{
    mat4 modelMatrix = useInstanceRendering ? instanceMatrix : model;
    
    gl_Position = projection * view * modelMatrix * vec4(pos, 1.0);
    
    TexCoord = uv;
    Normal = mat3(transpose(inverse(modelMatrix))) * normal;
    FragPos = (modelMatrix * vec4(pos, 1.0)).xyz;
    DirectionaLightPos = directionalLightProjection * modelMatrix * vec4(pos, 1.0);
}