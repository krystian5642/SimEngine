#version 460 core

layout (location = 0) in vec3 pos;

uniform mat4 model[100];
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model[gl_InstanceID] * vec4(pos, 1.0);
}