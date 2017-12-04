#include "GameObject.h"
#include "Component.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
:m_pDevice(pDevice)
{

}

Engine::CGameObject::~CGameObject(void)
{
	Release();
}

HRESULT Engine::CGameObject::Initialize(void)
{
	return S_OK;
}

int Engine::CGameObject::Update(void)
{
	map<const TCHAR*, CComponent*>::iterator iter		= m_mapComponent.begin();
	map<const TCHAR*, CComponent*>::iterator iter_end	= m_mapComponent.end();

	for(iter; iter != iter_end;)
	{	
		if(iter->second->Update() == 1)
		{
			Safe_Delete(iter->second);
			iter = m_mapComponent.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return 0;
}

void Engine::CGameObject::Render(void)
{

}

void Engine::CGameObject::Release(void)
{
	map<const TCHAR*, CComponent*>::iterator iter		= m_mapComponent.begin();
	map<const TCHAR*, CComponent*>::iterator iter_end	= m_mapComponent.end();

	DWORD dwRefCnt = 0;

	for(iter; iter != iter_end; ++iter)
	{
		dwRefCnt = iter->second->Release();

		if(dwRefCnt == 0)
		{
			Safe_Delete(iter->second);
		}
	}
	m_mapComponent.clear();
}

Engine::CComponent* Engine::CGameObject::GetComponent(TCHAR* pComponentKey)
{
	map<const TCHAR*, CComponent*>::iterator iter		= m_mapComponent.begin();
	map<const TCHAR*, CComponent*>::iterator iter_end	= m_mapComponent.end();

	map<const TCHAR*, CComponent*>::iterator find_iter = find_if(iter, iter_end, CTagFinder(pComponentKey));

	if(find_iter == m_mapComponent.end())
		return NULL;

	return find_iter->second;
}