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
texture g_BumpTexture;
texture g_SpecularTexture;
texture g_ShadowMap_Tex;

float4x4 g_matLightView;
float4x4 g_matLightProj;
float4x4 g_matLightViewProj;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
};

sampler BumpSampler = sampler_state
{
	texture = g_BumpTexture;
	magfilter = linear;
	minfilter = anisotropic;
	mipfilter = linear;
	MaxAnisotropy = 16;
};

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;
};

sampler2D ShdowSampler = sampler_state
{
	texture   = g_ShadowMap_Tex;
};

struct VS_IN
{
	vector vPosition	: POSITION;
	vector vNormal		: NORMAL;
	float2 vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector vPosition	: POSITION;
	vector vShade		: COLOR0; //명암
	vector vSpecular	: COLOR1;
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

	vector	vLightDirInv = normalize(g_vLightDir) * -1;
	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0) * (g_vLightDiffus * g_vMtrlDiffus) + (g_vLightAmbient * g_vMtrlAmbient);

	vector vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	vector vLook = vWorldPos - g_vCameraPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1), normalize(vReflect)), 0), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);

	return Out;
}

//기본 조명 쉐이더 VS

struct PS_IN
{
	vector		vShade		: COLOR0;
	vector		vSpecular	: COLOR1;
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

	Out.vColor = Out.vColor * In.vShade + In.vSpecular;

	return Out;
}

struct VS_IN_BUMP
{
	float4	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float3	vNormal		: NORMAL;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
};

struct VS_OUT_BUMP
{
	float4	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float3	vLightDir	: TEXCOORD1;
	float3	vViewDir	: TEXCOORD2;
	float3	T			: TEXCOORD3;
	float3	B			: TEXCOORD4;
	float3	N			: TEXCOORD5;
	float4	vClipPosition : TEXCOORD6;
};

VS_OUT_BUMP VS_MAIN_Bump(VS_IN_BUMP In)
{
	VS_OUT_BUMP Out = (VS_OUT_BUMP)0.f;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexCoord = In.vTexCoord;

	float4 worldPos = mul(In.vPosition, g_matWorld);
	float3 LightDir = g_vLightDir.xyz;
	Out.vLightDir = LightDir;

	float3 ViewDir = normalize(worldPos.xyz - g_vCameraPos.xyz);
	Out.vViewDir = ViewDir;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.N = worldNormal;

	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.T = worldTangent;

	float3 worldBinormal = mul(In.vBinormal, (float3x3)g_matWorld);
	Out.B = worldBinormal;

	Out.vClipPosition = mul(worldPos, g_matLightViewProj);
	
	return Out;
}

//노말 쉐이더 VS

float4 PS_MAIN_Bump(VS_OUT_BUMP In) : COLOR
{
	float4 Out = float4(0.f, 0.f, 0.f, 0.f);

	float3 tangectNormal = tex2D(BumpSampler, In.vTexCoord).xyz;

	tangectNormal = normalize(tangectNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangectNormal);

	float4 albedo = tex2D(BaseSampler, In.vTexCoord);
	float3 lightDir = normalize(In.vLightDir);
	float3 Diffuse = saturate(dot(worldNormal, -lightDir));
	Diffuse = g_vLightDiffus.xyz * albedo.rgb * Diffuse;

	float3 specular = 0;

	if(Diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 ViewDir = normalize(In.vViewDir);

		specular = saturate(dot(reflection, -ViewDir));
		specular = pow(specular, 20.f);

		float4 specularIntensity = tex2D(SpecularSampler, In.vTexCoord);
		specular *= specularIntensity.rgb * g_vLightDiffus.xyz;
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	Out = float4(ambient + Diffuse + specular, 1);

	float currentDept = In.vClipPosition.z / In.vClipPosition.w;
	float2 uv = In.vClipPosition.xy / In.vClipPosition.w;

	uv.y = - uv.y;
	uv = uv * 0.5 +0.5;

	if(0.0f <= uv.x && uv.x <= 1.0f && 0.0f <= uv.y && uv.y <= 1.0f)
	{
		float shadowDepth = tex2D(ShdowSampler, uv).r;

		if(currentDept > shadowDepth + 0.0000125f)
		{
			Out *= 0.5f;
		}
	}

	return Out;
}

technique Default_Technique
{
	pass Default
	{
		alphatestenable = false;
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}

	pass BumpMapping
	{
		alphatestenable = false;
		VertexShader	= compile vs_3_0 VS_MAIN_Bump();
		PixelShader		= compile ps_3_0 PS_MAIN_Bump();
	}
	
	pass BumpMapping_Alpha
	{	
		alphatestenable = true;
		alphafunc		= greater;
		Alpharef		= 0x0000005f;

		VertexShader	= compile vs_3_0 VS_MAIN_Bump();
		PixelShader		= compile ps_3_0 PS_MAIN_Bump();
	}
};