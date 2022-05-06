
struct Vout
{
	float2 tex_pos : TEXCOORD;
	float4 pos : SV_POSITION;
};

cbuffer cv
{
	matrix trans;
};

Vout main(float3 pos : POSITION, float2 tex_pos : TEXCOORD)
{
	Vout ot;
	ot.tex_pos = tex_pos;
	ot.pos = mul(float4(pos, 1.0f), trans);
	return ot;
}