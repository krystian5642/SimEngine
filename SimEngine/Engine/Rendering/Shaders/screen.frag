#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

void main() 
{
    const float gamma = 2.2;
    const float exposure = 1.0;
    
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
    
    hdrColor += bloomColor;
    
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    //result = pow(result, vec3(1.0 / gamma));
    
    FragColor = vec4(hdrColor, 1.0); //vec4(result, 1.0);
}