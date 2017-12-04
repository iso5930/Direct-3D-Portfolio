#include "ShaderSkinning.hpp"

float4x4 g_matWorld;
float4x4 g_matLightView;
float4x4 g_matLightProj;
float4x4 g_matLightViewProj;

float4	g_LightPosition;

struct VS_IN
{
	float4	vPosition	: POSITION;
	float4	vWeight		: BLENDWEIGHT0;
	int4	vIndices	: BLENDINDICES0;
};

struct VS_OUT
{
	float4 vPosition		: POSITION;
	float4 vClipPosition	: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);

	float fLastWeight = 0.f;

	int n = g_iInfluenceMax - 1;

	for(int i = 0; i < n; ++i)
	{
		fLastWeight		+= In.vWeight[i];
		vPos			+= In.vWeight[i] * mul(In.vPosition,   GetSkinMatrix(In.vIndices[i]));
	}

	fLastWeight = 1.f - fLastWeight;

	vPos  += fLastWeight * mul(In.vPosition,   GetSkinMatrix(In.vIndices[n]));
	vPos.w = 1.f;

	Out.vPosition = mul(vPos, g_matWorld);
	Out.vPosition = mul(Out.vPosition, g_matLightViewProj);
	
	Out.vClipPosition = Out.vPosition;

	return Out;
}

struct PS_INPUT
{
	float4 vClipPosition : TEXCOORD1;
};

float4 PS_MAIN(PS_INPUT Input) : COLOR
{
	float depth = Input.vClipPosition.z / Input.vClipPosition.w;

	return float4(depth.xxx, 1);
}

technique Default_Technique
{
	pass Default
	{      
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
}