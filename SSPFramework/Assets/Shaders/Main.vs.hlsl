#include "SSPFramework.gay"

struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};
 
struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput vert(VSInput i)
{
    VSOutput o = (VSOutput) 0;

    o.position = OceanObjectToClipSpace(float4(i.position, 1));
    return o;
}