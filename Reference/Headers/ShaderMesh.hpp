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

//Texture
texture g_BaseTexture;
texture g_BumpTexture;
texture g_CustomTexture;
texture g_SpecularTexture; 


float4 g_CustomColor_R;
float4 g_CustomColor_G;
float4 g_CustomColor_B;


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

sampler CustomSampler = sampler_state
{
	texture = g_CustomTexture;
	minfilter = linear;
	magfilter = linear;
};

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector vPosition	: POSITION;
	float2 vTexUV		: TEXCOORD0;

	//HW ½ºÅ°´×
	float4	vWeights	: BLENDWEIGHT0;
	int4	vIndices	: BLENDINDICES0;
};

struct VS_OUT
{
	vector vPosition	: POSITION;
	float2 vTexUV		: TEXCOORD0;
};	

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	//== HW ½ºÅ°´×
	float4	vPos		= float4(0.f, 0.f, 0.f, 1.f);
	float	lastWeight	= 0.f;

	int n = g_iInfluenceMax - 1;

	for(int i = 0; i < n; ++i)
	{
		lastWeight	+= In.vWeights[i];
		vPos		+= In.vWeights[i] * mul(In.vPosition,	GetSkinMatrix(In.vIndices[i]));
	}

	lastWeight = 1.f - lastWeight;

	vPos		+= lastWeight * mul(In.vPosition,	GetSkinMatrix(In.vIndices[n]));

	vPos.w = 1.f;

	//== HW ½ºÅ°´×

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(vector(vPos.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

//±âº» ½¦ÀÌ´õ VS

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

//±âº» ½¦ÀÌ´õ PS

struct VS_IN_BUMP
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

struct VS_OUT_BUMP
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
	//¸²¶óÀÌÆ®
};

VS_OUT_BUMP VS_MAIN_Bump(VS_IN_BUMP In)
{
	VS_OUT_BUMP Out = (VS_OUT_BUMP)0.f;
	
	//== HW ½ºÅ°´×

	float4	vPos		= float4(0.f, 0.f, 0.f, 1.f);
	float3	vTangent	= float3(0.0f, 0.0f, 0.0f);
	float3	vBinormal	= float3(0.0f, 0.0f, 0.0f);
	float3	vNormal		= float3(0.0f, 0.0f, 0.0f);

	float	lastWeight	= 0.f;
	int		n = g_iInfluenceMax - 1;

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

//³ë¸» ½¦ÀÌ´õ VS

float4 PS_MAIN_Bump(VS_OUT_BUMP In) : COLOR
{
	float3 vNormal = tex2D(BumpSampler, In.vTexCoord).xyz;

	vNormal = vNormal * 2.0f - 1.0f;
	vNormal = normalize(vNormal);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));

	TBN = transpose(TBN);

	vNormal = mul(TBN, vNormal);

	float4 albedo = tex2D(BaseSampler, In.vTexCoord);

	float3 LightDir = normalize(In.vLightDir);
	float3 Diffuse = pow(saturate(dot(vNormal, -LightDir)) * 0.5f + 0.5f, 1.f);

	Diffuse = g_vLightDiffus.xyz * albedo.xyz * Diffuse;

	
	float3 Specular = 0;

	if(Diffuse.x > 0)
	{
		float3 Reflection = reflect(LightDir, vNormal);
		float3 ViewDir = normalize(In.vViewDir);

		Specular = saturate(dot(Reflection, -ViewDir));
		Specular = pow(Specular, 20.0f);

		float4 SpeculatIntensity = tex2D(SpecularSampler, In.vTexCoord);
		Specular *= SpeculatIntensity.xyz * g_vLightDiffus.xyz;
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	return float4(ambient + Diffuse + Specular, albedo.a);

	//float3 tangectNormal = tex2D(BumpSampler, In.vTexCoord).xyz;

	//tangectNormal = normalize(tangectNormal * 2 - 1);

	//float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	//TBN = transpose(TBN);
	//float3 worldNormal = mul(TBN, tangectNormal);

	//float4 albedo = tex2D(BaseSampler, In.vTexCoord);

	//float3 lightDir = normalize(In.vLightDir);
	//float3 Diffuse = pow(saturate(dot(worldNormal, -lightDir)) * 0.5f + 0.5f, 1.f);
	////float3 Diffuse = dot(worldNormal, -lightDir);
	//Diffuse = g_vLightDiffus.xyz * albedo.rgb * Diffuse;

	//float3 specular = 0;

	//if(Diffuse.x > 0)
	//{
	//	float3 reflection = reflect(lightDir, worldNormal);
	//	float3 ViewDir = normalize(In.vViewDir);

	//	specular = saturate(dot(reflection, -ViewDir));
	//	specular = pow(specular, 20.f);

	//	float4 specularIntensity = tex2D(SpecularSampler, In.vTexCoord);
	//	specular *= specularIntensity.rgb * g_vLightDiffus.xyz;
	//}

	//	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	////========= Rim

	//vector worldPos = In.vPosition2;
	//vector viewDir2 = normalize(g_vCameraPos - worldPos);

	//half rim = 1.0 - saturate(dot(viewDir2, In.vNormal));

	//vector Emission = g_vLightDiffus * pow(rim, 7.f);

	////========= Rim

	//return float4(ambient + Diffuse + specular, albedo.a) + float4(Emission.xyz, albedo.a);
}

//³ë¸» ½¦ÀÌ´õ PS

float4 PS_MAIN_CustomColor_Bump(VS_OUT_BUMP In) : COLOR
{
	float3 tangectNormal = tex2D(BumpSampler, In.vTexCoord).xyz;

	tangectNormal = normalize(tangectNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangectNormal);

	float4 albedo = tex2D(BaseSampler, In.vTexCoord);
	float fAlpth = albedo.a;
	float4 texCustom = tex2D(CustomSampler, In.vTexCoord);
	
	if(albedo.r > max(albedo.g, albedo.b))
	{
		albedo = 1.5f * texCustom * g_CustomColor_R;
	}
	else if(albedo.g > max(albedo.r, albedo.b))
	{
		albedo = 1.5f * texCustom * g_CustomColor_G;
	}
	else if(albedo.b > max(albedo.r, albedo.g))
	{
		albedo = 1.5f * texCustom * g_CustomColor_B;
	}

	float3 lightDir = normalize(In.vLightDir);
	float3 Diffuse = pow(saturate(dot(worldNormal, -lightDir)) * 0.5f + 0.5f, 1.f);
	//float3 Diffuse = dot(worldNormal, -lightDir);
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

	//========= Rim

	vector worldPos = In.vPosition2;
	vector viewDir2 = normalize(g_vCameraPos - worldPos);

	half rim = 1.0 - saturate(dot(viewDir2, In.vNormal));

	vector Emission = g_vLightDiffus * pow(rim, 7.f);

	//========= Rim

	return float4(ambient + Diffuse + specular, fAlpth) + float4(Emission.xyz, fAlpth);
}

//¿°»ö ½¦ÀÌ´õ + ³ë¸» + ½ºÆåÅ§·¯

technique Default_Technique
{
	pass Default
	{	
		alphatestenable = false;

		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}

	pass AlphaTest
	{
		alphatestenable = true;
		alphafunc		= greater;
		Alpharef		= 0x0000005f;
		CULLMODE		= none;

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
		CULLMODE		= none;

		VertexShader	= compile vs_3_0 VS_MAIN_Bump();
		PixelShader		= compile ps_3_0 PS_MAIN_Bump();
	}
	
	pass CustomColor_BumpMapping
	{
		alphatestenable = false;

		VertexShader	= compile vs_3_0 VS_MAIN_Bump();
		PixelShader		= compile ps_3_0 PS_MAIN_CustomColor_Bump();
	}

	pass CustomColor_BumpMapping_Alpha
	{
		alphatestenable = true;
		alphafunc		= greater;
		Alpharef		= 0x0000005f;

		VertexShader	= compile vs_3_0 VS_MAIN_Bump();
		PixelShader		= compile ps_3_0 PS_MAIN_CustomColor_Bump();
	}
};