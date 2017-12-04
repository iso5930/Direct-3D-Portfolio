#include "LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr);

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Release();
}

HRESULT Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, const WORD& wLightIdx)
{
	NULL_CHECK_RETURN(pDevice, E_FAIL);
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	CLight*	pLight = CLight::Create(pDevice, pLightInfo, wLightIdx);

	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_Lightlist.push_back(pLight);

	return S_OK;
}	

void Engine::CLightMgr::Release(void)
{
	list<CLight*>::iterator iter = m_Lightlist.begin();
	list<CLight*>::iterator iter_end = m_Lightlist.end();

	for_each(iter, iter_end, CDeleteObj());
	m_Lightlist.clear();
}

const D3DLIGHT9* Engine::CLightMgr::GetLightInfo(const UINT& iIndex)
{
	list<CLight*>::iterator iter = m_Lightlist.begin();

	for(size_t i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	return (*iter)->GetLightInfo();
}