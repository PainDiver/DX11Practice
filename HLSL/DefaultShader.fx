Texture2D txDiffuse : register(t0);

Texture2D txNormal : register(t1);

SamplerState samLinear: register(s0);

cbuffer worldMat : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	matrix wvp;
};

cbuffer DirectionalLight : register(b1)
{
	float4 dir;
	float4 lightColor;
}

cbuffer flags : register(b2)
{
	int instancing;
	int padding[3];
};

cbuffer InstancingInfo : register(b3)
{
	matrix instanceWorld[200];
}


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
	float3 viewNormal : NORMAL;
	float3 viewTangent :TANGENT;
	float3 viewBinormal : BINORMAL;
	float3 viewPos: POSITION;

};



PS_INPUT VS (VS_INPUT input, uint instID : SV_Instanceid)
{
	PS_INPUT ps = (PS_INPUT)0;


	if (instancing == 1)
	{
		ps.Pos = mul(float4(input.Pos, 1.f), instanceWorld[instID]);
		ps.Pos = mul(ps.Pos, view);
		ps.Pos = mul(ps.Pos, proj);
		ps.viewPos = mul(float4(input.Pos, 1.f), instanceWorld[instID]);
		ps.viewPos = mul(float4(ps.viewPos, 1.f), view);
	}
	else
	{
		ps.Pos = mul(float4(input.Pos, 1.f), wvp);
		ps.viewPos = mul(float4(input.Pos, 1.f), world);
		ps.viewPos = mul(float4(ps.viewPos, 1.f), view);
	}

	ps.viewNormal = mul(input.Normal, world);
	ps.viewNormal = mul(input.Normal, view);

	ps.viewTangent = mul(input.Tangent, world);
	ps.viewTangent = mul(input.Tangent, view);

	ps.viewBinormal = normalize(cross(ps.viewTangent, ps.viewNormal));
	ps.viewBinormal = normalize(cross(ps.viewTangent, ps.viewNormal));

	ps.Tex = input.Tex;

	return ps;
}


float4 PS(PS_INPUT input) : SV_Target0
{
	float4 textureColor = txDiffuse.Sample(samLinear, input.Tex);
	float3 normalTexture = txNormal.Sample(samLinear, input.Tex).xyz;
	
	//normal parsing
	normalTexture = (normalTexture -0.5f) * 2.0f; // 이미지화된 노멀을 좌표계 범위로 이동시킴 0~1 -> -1 ~1
	float3x3 tbnMat = {input.viewTangent, input.viewBinormal, input.viewNormal}; //물체의 월드기준 탄젠트스페이스로 이동시켜 접선이 월드에서도 유효한 노멀생성
	normalTexture = normalize(mul(normalTexture, tbnMat));

	//diffuse
	float4 viewDir =mul(dir, view);
	float cos = saturate(dot(normalTexture, -dir));

	//specular
	float3 eye = normalize(input.viewPos);
	float3 reflection = normalize(viewDir + saturate(2 * dot(-viewDir,normalTexture)*normalTexture) );
	float4 reflectionCos = saturate(dot(reflection, -eye));

	float4 specular = pow(reflectionCos, 3);
	//ambient
	float4 ambient = 0.1;
	float4 diffuse = saturate(mul(textureColor, cos));

	float4 color = (diffuse * lightColor + ambient * lightColor)  + specular;



	return color;
}




