#include "ShaderBase.hpp"
#include "ShaderSkinning.hpp"

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

float	g_fAlpha = 1.f;

//Texture
texture g_BaseTexture;
texture g_BumpTexture;
texture g_SpecularTexture;

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

struct VS_IN
{
	float4	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float3	vNormal		: NORMAL;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;

	//HW ½ºÅ°´×
	float4	vWeights	: BLENDWEIGHT0;
	int4	vIndices	: BLENDINDICES0;
};

struct VS_OUT
{
	float4	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float3	vLightDir	: TEXCOORD1;
	float3	vViewDir	: TEXCOORD2;
	float3	T			: TEXCOORD3;
	float3	B			: TEXCOORD4;
	float3	N			: TEXCOORD5;
	float4	vPosition2	: TEXCOORD6;
	float3	vNormal		: NORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0.f;

	//== HW ½ºÅ°´×
	float4	vPos		= float4(0.f, 0.f, 0.f, 1.f);
	float3	vTangent	= float3(0.0f, 0.0f, 0.0f);
	float3	vBinormal	= float3(0.0f, 0.0f, 0.0f);
	float3	vNormal		= float3(0.0f, 0.0f, 0.0f);
	float	lastWeight	= 0.f;

	int n = g_iInfluenceMax - 1;

	for(int i = 0; i < n; ++i)
	{
		lastWeight	+= In.vWeights[i];
		vPos		+= In.vWeights[i] * mul(In.vPosition,	GetSkinMatrix(In.vIndices[i]));
		vTangent	+= In.vWeights[i] * mul(In.vTangent, (float3x3)GetSkinMatrix(In.vIndices[i]));
		vBinormal	+= In.vWeights[i] * mul(In.vBinormal, (float3x3)GetSkinMatrix(In.vIndices[i]));
		vNormal		+= In.vWeights[i] * mul(In.vNormal, (float3x3)GetSkinMatrix(In.vIndices[i]));
	}

	lastWeight = 1.f - lastWeight;

	vPos		+= lastWeight * mul(In.vPosition,	GetSkinMatrix(In.vIndices[n]));
	vTangent	+= lastWeight * mul(In.vTangent, (float3x3)GetSkinMatrix(In.vIndices[n]));
	vBinormal	+= lastWeight * mul(In.vBinormal, (float3x3)GetSkinMatrix(In.vIndices[n]));
	vNormal		+= lastWeight * mul(In.vNormal, (float3x3)GetSkinMatrix(In.vIndices[n]));

	vPos.w = 1.f;

	//== HW ½ºÅ°´×

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vPos, matWVP);
	Out.vTexCoord = In.vTexCoord;

	float4 worldPos = mul(vPos, g_matWorld);
	float3 LightDir = g_vLightDir.xyz;
	Out.vLightDir = LightDir;

	float3 ViewDir = normalize(worldPos.xyz - g_vCameraPos.xyz);
	Out.vViewDir = ViewDir;

	float3 worldNormal = mul(vNormal, (float3x3)g_matWorld);
	Out.N = worldNormal;

	float3 worldTangent = mul(vTangent, (float3x3)g_matWorld);
	Out.T = worldTangent;

	float3 worldBinormal = mul(vBinormal, (float3x3)g_matWorld);
	Out.B = worldBinormal;

	Out.vPosition2 = worldPos;
	Out.vNormal = mul(vNormal, (float3x3)g_matWorld);

	return Out;
}

float4 PS_MAIN(VS_OUT In) : COLOR
{
	float3 tangectNormal = tex2D(BumpSampler, In.vTexCoord).xyz;
	tangectNormal = normalize(tangectNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangectNormal);

	float4 albedo = tex2D(BaseSampler, In.vTexCoord);

	float3 lightDir = normalize(In.vLightDir);
	float3 Diffuse = pow(saturate(dot(worldNormal, -lightDir)) * 0.5f + 0.5f, 1.f);
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

	vector worldPos = In.vPosition2;
	vector viewDir2 = normalize(g_vCameraPos - worldPos);

	half rim = 1.0 - saturate(dot(viewDir2, In.vNormal));

	vector Emission = g_vLightDiffus * pow(rim, 7.f);

	return float4(ambient + Diffuse + specular, 1.f) + Emission;
}

technique Default_Technique
{
	pass Default
	{	
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
}