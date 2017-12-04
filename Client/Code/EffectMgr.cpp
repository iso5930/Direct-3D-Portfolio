#include "stdafx.h"
#include "EffectMgr.h"
#include "Effect.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CEffectMgr);

CEffectMgr::CEffectMgr(void)
:m_pDevice(Engine::Get_GraphicDev()->GetDevice())
{

}

CEffectMgr::~CEffectMgr(void)
{
	Release();
}

void CEffectMgr::AddEffect(CEffect* pEffect)
{
	m_EffectList.push_back(pEffect);
}

void CEffectMgr::Update(void)
{
	list<CEffect*>::iterator iter = m_EffectList.begin();
	list<CEffect*>::iterator iter_end = m_EffectList.end();

	for(iter; iter != iter_end;)
	{
		if((*iter)->Update() == 1)
		{
			Engine::Safe_Delete((*iter));
			iter = m_EffectList.erase(iter);
		}
		else
			++iter;
	}
}

void CEffectMgr::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	list<CEffect*>::iterator iter = m_EffectList.begin();
	list<CEffect*>::iterator iter_end = m_EffectList.end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CEffectMgr::Release(void)
{	
	list<CEffect*>::iterator iter = m_EffectList.begin();
	list<CEffect*>::iterator iter_end = m_EffectList.end();

	for(iter; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete((*iter));
	}
	m_EffectList.clear();
}