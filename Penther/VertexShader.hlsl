
struct Vout
{
	float3 col : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer cv
{
	matrix trans;
};

Vout main(float3 pos : POSITION , float3 col : COLOR)
{
	Vout ot;
	ot.col = col;
	ot.pos = mul(float4(pos, 1.0f) , trans);
	return ot;
}