#include "SkillMgr.h"
#include "GameObject.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(Engine::CSkillMgr)

Engine::CSkillMgr::CSkillMgr(void)
{
	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
}

Engine::CSkillMgr::~CSkillMgr(void)
{
	Release();
}

void Engine::CSkillMgr::AddSkill(CGameObject* pSkill)
{
	m_SkillList.push_back(pSkill);
}

void Engine::CSkillMgr::Update(void)
{
	list<CGameObject*>::iterator iter = m_SkillList.begin();
	list<CGameObject*>::iterator iter_end = m_SkillList.end();

	for(iter; iter != iter_end;)
	{
		if((*iter)->Update() == 1)
		{
			Safe_Delete((*iter));
			iter = m_SkillList.erase(iter);
		}
		else
			++iter;
	}

	iter = m_KadanSkillList.begin();
	iter_end = m_KadanSkillList.end();

	for(iter; iter != iter_end;)
	{
		if((*iter)->Update() == 1)
		{
			Safe_Delete((*iter));
			iter = m_KadanSkillList.erase(iter);
		}
		else
			++iter;
	}
}

void Engine::CSkillMgr::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	list<CGameObject*>::iterator iter = m_SkillList.begin();
	list<CGameObject*>::iterator iter_end = m_SkillList.end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	iter = m_KadanSkillList.begin();
	iter_end = m_KadanSkillList.end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}	

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Engine::CSkillMgr::Release(void)
{
	list<CGameObject*>::iterator iter = m_SkillList.begin();
	list<CGameObject*>::iterator iter_end = m_SkillList.end();

	for(iter; iter != iter_end; ++iter)
	{
		Safe_Delete((*iter));
	}	
	m_SkillList.clear();

	iter = m_KadanSkillList.begin();
	iter_end = m_KadanSkillList.end();

	for(iter; iter != iter_end; ++iter)
	{
		Safe_Delete((*iter));
	}	
	m_KadanSkillList.clear();
}

list<Engine::CGameObject*>* Engine::CSkillMgr::GetList(void)
{
	return &m_SkillList;
}

void Engine::CSkillMgr::AddKadanSkill(CGameObject* pSkill)
{
	m_KadanSkillList.push_back(pSkill);
}

list<Engine::CGameObject*>* Engine::CSkillMgr::GetKadanSkill(void)
{
	return &m_KadanSkillList;
}