struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 col : SV_TARGET0;
};

Texture2D _MainTex : register(t0);
sampler _LinearSampler : register(s0);

PSOutput frag(PSInput i)
{
    PSOutput o = (PSOutput)0;
    o.col = _MainTex.Sample(_LinearSampler, i.uv);
    return o;
}