#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

layout (location = 4) in mat4 instanceMatrix;

out vec2 TexCoord;
out mat3 TBN;
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
    FragPos = (modelMatrix * vec4(pos, 1.0)).xyz;
    DirectionaLightPos = directionalLightProjection * modelMatrix * vec4(pos, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    Normal = normalize(normalMatrix * normal);
    vec3 Tangent = normalize(normalMatrix * tangent);
    
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    
    vec3 Bitangent = cross(Tangent, Normal);

    TBN = mat3(Tangent, Bitangent, Normal);
}