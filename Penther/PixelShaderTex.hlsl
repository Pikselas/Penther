Texture2D tex;
SamplerState sam;

float4 main(float2 tex_pos : TEXCOORD) : SV_TARGET
{
	return tex.Sample(sam , tex_pos);
}