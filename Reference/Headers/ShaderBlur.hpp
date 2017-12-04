float2 g_vCenter = { 0.5f, 0.5f };
float g_fBlurStart = 1.f;
float g_fBlurWidth = -0.1f;
int g_iSamplesForRadialBlur = 10;

texture g_SceneTexture;
sampler SceneSampler = sampler_state
{
	texture = g_SceneTexture;
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN 
{
	float4 vPosition : POSITION;
	float2 vTexUV	: TEXCOORD0;
};

struct VS_OUT 
{
	float4 vPosition : POSITION;
	float2 vTexUV	: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;

	Out.vPosition = In.vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vTexUV = In.vTexUV - g_vCenter;

	float fScale = 0.f;
	
	for(int i = 0; i < g_iSamplesForRadialBlur; i++)
	{
		fScale = g_fBlurStart + g_fBlurWidth * 0.1f * 
			(i / (float)(g_iSamplesForRadialBlur - 1));

		Out.vColor += tex2D(SceneSampler, vTexUV * fScale + g_vCenter);
	}

	Out.vColor /= g_iSamplesForRadialBlur;

	return Out;
}

technique Tech0
{
	pass Pass0
	{
		alphablendenable = false;
		alphatestenable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}