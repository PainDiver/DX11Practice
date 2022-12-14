Texture2D txDiffuse : register(t0);

SamplerState samLinear: register(s0);

cbuffer worldMat : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	matrix wvp;
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent :TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent :TANGENT;
	float3 Binormal : BINORMAL;
	float3 viewPos: POSITION;
};



PS_INPUT VS (VS_INPUT input)
{
	PS_INPUT ps = (PS_INPUT)0;
	ps.Pos =mul(float4( input.Pos,1.f),wvp);

	ps.Tex = input.Tex;

	return ps;
}


float4 PS(PS_INPUT input) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, input.Tex);

	return color;
}




