struct PSInput
{
    float4 position : SV_POSITION;
};

struct PSOutput
{
    float4 color : SV_TARGET0;
};

PSOutput frag(PSInput i)
{
    PSOutput o = (PSOutput) 0;
    o.color = float4(1, 1, 1, 0);
    return o;
}