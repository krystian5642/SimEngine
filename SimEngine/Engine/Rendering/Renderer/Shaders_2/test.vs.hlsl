struct VSOutput
{
    float4 position : SV_POSITION;
    [[vk::location(0)]] float3 color : COLOR0;
};

static const float3 positions[3] =
{
    float3( 0.0, -0.4, 0.0),
    float3( 0.4,  0.4, 0.0),
    float3(-0.4,  0.4, 0.0)
};

static const float3 colours[3] =
{
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

VSOutput main(uint vertexID : SV_VertexID)
{
    VSOutput output;
    output.position = float4(positions[vertexID], 1.0);
    output.color = colours[vertexID];
    return output;
}