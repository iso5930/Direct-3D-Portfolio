#include "ShaderBase.hpp"

//Light
vector g_vLightDir;
vector g_vLightDiffus;
vector g_vLightAmbient;
vector g_vLightSpecular;

//Material
vector	g_vMtrlDiffus;
vector	g_vMtrlAmbient;
vector	g_vMtrlSpecular;
float	g_fPower = 1.f;

vector  g_vCameraPos;

//Texture
texture g_BaseTexture;

float	g_fAlpha = 1.f;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector vPosition	: POSITION;
	float2 vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector vPosition	: POSITION;
	float2 vTexUV		: TEXCOORD0;
};	

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

technique Default_Technique
{
	pass Default
	{	
		alphatestenable = true;
		alphafunc		= greater;
		Alpharef		= 0x0000005f;

		CULLMODE		= none;
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
}