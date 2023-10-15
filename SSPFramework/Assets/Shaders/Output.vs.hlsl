struct VSInput
{

};

struct VSOutput
{
    float4 position: SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOutput vert(uint id : SV_VertexID)
{
    VSOutput o = (VSOutput)0;

	o.uv = float2((id << 1) & 2, id & 2);
	o.position = float4(o.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    return o;
}