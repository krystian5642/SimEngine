#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

struct FXAASettings
{
    float FXAASpanMax;
    float FXAAReduceMin;
    float FXAAReduceMul;
};

uniform sampler2D screenTexture;
uniform vec2 texelSize;
uniform FXAASettings fxaaSettings;

void main() 
{
    vec3 luma = vec3(0.299, 0.587, 0.114);
    
    float lumaTL = dot(luma, texture(screenTexture, TexCoords + vec2(-1.0, -1.0) * texelSize).xyz);
    float lumaTR = dot(luma, texture(screenTexture, TexCoords + vec2(1.0, -1.0) * texelSize).xyz);
    float lumaBL = dot(luma, texture(screenTexture, TexCoords + vec2(-1.0, 1.0) * texelSize).xyz);
    float lumaBR = dot(luma, texture(screenTexture, TexCoords + vec2(1.0, 1.0) * texelSize).xyz);
    float lumaM  = dot(luma, texture(screenTexture, TexCoords).xyz);
    
    vec2 blurDir;
    blurDir.x = ((lumaTL + lumaTR) - (lumaBL + lumaBR));
    blurDir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
    
    float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (fxaaSettings.FXAAReduceMul * 0.25), fxaaSettings.FXAAReduceMin);
    float inverseDirAdjustment = 1.0 / (min(abs(blurDir.x), abs(blurDir.y)) + dirReduce);
    blurDir = clamp(blurDir * inverseDirAdjustment, vec2(-fxaaSettings.FXAASpanMax), vec2(fxaaSettings.FXAASpanMax)) * texelSize;
    
    vec3 rgbA = (1.0 / 2.0) * (
            texture(screenTexture, TexCoords + blurDir * (1.0 / 3.0 - 0.5)).xyz
            +  texture(screenTexture, TexCoords + blurDir * (2.0 / 3.0 - 0.5)).xyz
        );
    
    vec3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
        texture(screenTexture, TexCoords + blurDir * (0.0 / 3.0 - 0.5)).xyz
        +  texture(screenTexture, TexCoords + blurDir * (3.0 / 3.0 - 0.5)).xyz
    );
    
    float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
    float lumaMax = max(lumaM, max(min(lumaTL, lumaTR), max(lumaBL, lumaBR)));
    
    float lumaResult = dot(luma, rgbB);
    
    if (lumaResult < lumaMin || lumaResult > lumaMax)
    {
        FragColor = vec4(rgbA, 1.0);
    }
    else
    {
        FragColor = vec4(rgbB, 1.0);
    }
}