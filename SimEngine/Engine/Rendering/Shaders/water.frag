#version 460 core

in vec3 Position;
in vec3 Normal;

out vec4 color;

uniform samplerCube skybox;
uniform vec3 cameraPosition;

void main() 
{
    float ratio = 1.00 / 2.42;
    vec3 I = normalize(Position - cameraPosition);
    vec3 R = refract(I, normalize(Normal), ratio);
    color = vec4(texture(skybox, R).rgb, 1.0);
}