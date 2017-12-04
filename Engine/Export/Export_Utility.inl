
CManagement* Get_Management(void)
{	
	return CManagement::GetInstance();
}

CLightMgr* Get_LightMgr(void)
{
	return CLightMgr::GetInstance();
}

CShaderMgr* Get_ShaderMgr(void)
{
	return CShaderMgr::GetInstance();
}

CNaviMgr* Get_NaviMgr(void)
{
	return CNaviMgr::GetInstance();
}

CSkillMgr* Get_SkillMgr(void)
{
	return CSkillMgr::GetInstance();
}

CShadowMgr* Get_ShadowMgr(void)
{
	return CShadowMgr::GetInstance();
}