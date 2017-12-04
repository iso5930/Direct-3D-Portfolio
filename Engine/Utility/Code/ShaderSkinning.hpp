texture g_HWSkinningTexture;

int		g_iInfluenceMax;

sampler HWSkinningSampler = sampler_state
{
	texture = g_HWSkinningTexture;
};

float4x4 GetSkinMatrix(int idx)
{
	float4 uvCol = float4(((float)((idx % 16) * 4) + 0.5f) / 64.0f, ((float)((idx / 16)) + 0.5f) / 64.0f, 0.0f, 0.0f);

	float4x4 mat = { tex2Dlod(HWSkinningSampler, uvCol),
		tex2Dlod(HWSkinningSampler, uvCol + float4(1.0f / 64.0f, 0, 0, 0)),
		tex2Dlod(HWSkinningSampler, uvCol + float4(2.0f / 64.0f, 0, 0, 0)),
		tex2Dlod(HWSkinningSampler, uvCol + float4(3.0f / 64.0f, 0, 0, 0)) };

	return mat;
}