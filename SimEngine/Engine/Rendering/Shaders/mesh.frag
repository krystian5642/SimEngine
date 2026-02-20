#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionaLightPos;

out vec4 color;

const int dirLightMaxCount = 10;
const int pointLightMaxCount = 5;
const int spotLightMaxCount = 5;

struct LightData
{
    vec3 color;
    float ambient;
    float diffuse;
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

struct SpotLight
{
    LightData lightData;
    vec3 position;
    vec3 direction;
    float innerConeAngle;
    float outerConeAngle;
};

struct OmniShadowMap
{
    samplerCube shadowMap;
    float farPlane;
};

struct Material
{
    sampler2D texture;
    bool useTexture;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform int dirLightCount;
uniform int pointLightCount;
uniform int spotLightCount;

uniform sampler2D dirShadowMaps[dirLightMaxCount];
uniform OmniShadowMap omniShadowMaps[pointLightMaxCount + spotLightMaxCount];

uniform DirectionalLight dirLights[dirLightMaxCount];
uniform PointLight pointLights[pointLightMaxCount];
uniform SpotLight spotLights[spotLightMaxCount];

uniform vec3 cameraPosition;

float CalcShadowFactor(int lightIndex, sampler2D dirShadowMap)
{
    DirectionalLight dirLight = dirLights[lightIndex];
    
    vec3 projLightCoords = DirectionaLightPos.xyz / DirectionaLightPos.w;
    projLightCoords = (projLightCoords * 0.5) + 0.5;
    
    if(projLightCoords.z > 1.0)
    {
        return 0.0;
    }
    
    float bias = max(0.01 * (1.0 - dot(normalize(-dirLight.direction), normalize(Normal))), 0.001);
    
    vec2 texelSize = 1.0 / textureSize(dirShadowMap, 0);
    float shadowFactor = 0.0;
    for(int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            float closest = texture(dirShadowMap, projLightCoords.xy + vec2(i, j) * texelSize).r;
            shadowFactor += closest < projLightCoords.z - bias ? 1.0 : 0.0;
        }
    }

    shadowFactor /= 9.0;
    return shadowFactor;
}

float CalcOmniShadowFactor(OmniShadowMap omniShadowMap, vec3 position)
{
    vec3 fragToLight = FragPos - position;
    float distance = length(fragToLight);
    float currentDepth = distance / omniShadowMap.farPlane;
    
    if(distance > omniShadowMap.farPlane)
    {
        return 0.0;
    }
    
    float bias = max(0.01 * (1.0 - dot(normalize(fragToLight), normalize(Normal))), 0.001);
    
    float shadowFactor = 0.0;
    float samples = 4.0;
    float offset = 0.1;
    
    for(float x = -offset; x < offset; x+= offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y+= offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z+= offset / (samples * 0.5))
            {
                float closest = texture(omniShadowMap.shadowMap, fragToLight + vec3(x, y, z)).r;
                shadowFactor += closest < currentDepth - bias ? 1.0 : 0.0;
            }
        }
    }

    shadowFactor /= pow(samples, 3.0);

    return shadowFactor;
}

vec4 CalcLightColorByDirection(LightData lightData, vec3 direction, float shadowFactor)
{
    vec4 materialAmbient = material.useTexture ? texture(material.texture, TexCoord) : vec4(material.ambient, 1.0);
    vec4 ambientColor = vec4(lightData.color * lightData.ambient, 1.0) * materialAmbient;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(direction);
    
    vec4 materialDiffuse = material.useTexture ? texture(material.texture, TexCoord) : vec4(material.diffuse, 1.0);
    float diffuseFactor = max(dot(lightDir, normal), 0.0) * lightData.diffuse;
    vec4 diffuseColor = vec4(diffuseFactor * lightData.color, 1.0) * materialDiffuse;
    
    vec4 specularColor = {0.0, 0.0, 0.0, 0.0};
    if (diffuseFactor > 0.0)
    {
        vec3 reflectLight = reflect(-lightDir, normal);
        vec3 view = normalize(cameraPosition - FragPos);
        float shininessFactor = dot(reflectLight, view);
        if(shininessFactor > 0.0)
        {
            shininessFactor = pow(shininessFactor, material.shininess);
            specularColor = vec4(lightData.color * material.specular * shininessFactor, 1.0);
        }
    }
    return ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor);
}

vec4 CalcDirectionalLightColor()
{
    vec4 color = {0.0, 0.0, 0.0, 0.0};
    
    for(int i = 0; i < dirLightCount; i++)
    {
        DirectionalLight dirLight = dirLights[i];
        
        float shadowFactor = CalcShadowFactor(i, dirShadowMaps[i]);
        color += CalcLightColorByDirection(dirLight.lightData, -dirLight.direction, shadowFactor);
    }
    
    return color;
}

vec4 CalcPointLightColor()
{
    vec4 color = {0.0, 0.0, 0.0, 0.0};

    for(int i = 0; i < pointLightCount; i++)
    {
        PointLight pointLight = pointLights[i];
        
        float distanceSqrt = dot(FragPos - pointLight.position, FragPos - pointLight.position);
        vec3 direction = normalize(pointLight.position - FragPos);
        
        LightData lightData;
        lightData.color = pointLight.lightData.color;
        lightData.ambient = pointLight.lightData.ambient / distanceSqrt;
        lightData.diffuse = pointLight.lightData.diffuse / distanceSqrt;
        
        float shadowFactor = CalcOmniShadowFactor(omniShadowMaps[i], pointLight.position);
        color += CalcLightColorByDirection(lightData, direction, shadowFactor);
    }
    
    return color;
}

vec4 CalcSpotLightColor()
{
    vec4 color = {0.0, 0.0, 0.0, 0.0};

    vec3 normal = normalize(Normal);
    
    for(int i = 0; i < spotLightCount; i++)
    {
        SpotLight spotLight = spotLights[i];

        vec3 lightDirection = spotLight.position - FragPos;
        float distanceSqrt = dot(lightDirection, lightDirection);
        float diffuse = spotLight.lightData.diffuse / distanceSqrt;
        float ambient = spotLight.lightData.ambient / distanceSqrt;
        
        vec3 lightDir = normalize(lightDirection);
        vec3 reflectLight = reflect(-lightDir, normal);
        float diffuseFactor = max(dot(reflectLight, normal), 0.0);
        
        float angle = dot(normalize(spotLight.direction), normalize(-lightDirection));
        float inner = cos(spotLight.innerConeAngle);
        float outer = cos(spotLight.outerConeAngle);

        vec4 materialDiffuse = material.useTexture ? texture(material.texture, TexCoord) : vec4(material.diffuse, 1.0);
        float falloff = clamp((angle - outer) / (inner - outer), 0.0, 1.0);
        
        float shadowFactor = CalcOmniShadowFactor(omniShadowMaps[i + pointLightCount], spotLight.position);
        color += vec4(spotLight.lightData.color, 0.0) * ((1.0 - shadowFactor) * diffuse * diffuseFactor * falloff * materialDiffuse + ambient * vec4(material.ambient, 1.0));
    }

    return color;
}

void main()
{
    vec4 directionalLightColor = CalcDirectionalLightColor();
    vec4 pointLightColor = CalcPointLightColor();
    vec4 spotLightColor = CalcSpotLightColor();
    
    color = directionalLightColor + pointLightColor + spotLightColor;

    float gamma = 2.2;
    color = vec4(pow(color.rgb, vec3(1.0 / gamma)), color.a);
}










