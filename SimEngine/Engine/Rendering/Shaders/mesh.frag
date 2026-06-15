#version 460 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct LightData
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    LightData lightData;
    vec3 direction;
};

struct PointLight
{
    LightData lightData;
    vec3 position;
};

struct Material
{
    vec3 color;
};

uniform Material material;

uniform int dirLightCount;
uniform int pointLightCount;

const int dirLightMaxCount = 10;
const int pointLightMaxCount = 5;

uniform DirectionalLight dirLights[dirLightMaxCount];
uniform PointLight pointLights[pointLightMaxCount];

vec3 CalcLightColorByDirection(LightData lightData, vec3 direction)
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(direction);
    
    float diffuseFactor = max(dot(lightDir, normal), 0.0) * lightData.diffuseIntensity;
    vec3 diffuseColor = diffuseFactor * lightData.color * material.color;
    
    vec3 ambientColor = lightData.ambientIntensity * lightData.color * material.color;
    
    return ambientColor + diffuseColor;
}

vec3 CalcDirectionalLightColor()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < dirLightCount; i++)
    {
        DirectionalLight dirLight = dirLights[i];
        color += CalcLightColorByDirection(dirLight.lightData, -dirLight.direction);
    }
    
    return color;
}

vec3 CalcPointLightColor()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < pointLightCount; i++)
    {
        PointLight pointLight = pointLights[i];

        float distanceSqrt = dot(FragPos - pointLight.position, FragPos - pointLight.position);
        vec3 direction = normalize(pointLight.position - FragPos);

        LightData lightData;
        lightData.color = pointLight.lightData.color;
        lightData.ambientIntensity = pointLight.lightData.ambientIntensity / distanceSqrt;
        lightData.diffuseIntensity = pointLight.lightData.diffuseIntensity / distanceSqrt;
        
        color += CalcLightColorByDirection(lightData, direction);
    }

    return color;
}

void main()
{
    vec3 directionalLightColor = CalcDirectionalLightColor();
    vec3 pointLightColor = CalcPointLightColor();

    FragColor = vec4(directionalLightColor + pointLightColor, 1.0);
}