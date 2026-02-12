#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float kernel[9] = float[]( 
    1.0, 1.0, 1.0, 
    1.0, -8.0, 1.0, 
    1.0, 1.0, 1.0 
);

const float offset = 1.0 / 100.0;

void main() 
{
    FragColor = texture(screenTexture, TexCoords);
    
/*    int kernelIndex = 0;
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            FragColor += kernel[kernelIndex] * texture(screenTexture, TexCoords + offset * vec2(x, y));
            kernelIndex++;
        }
    }*/
}