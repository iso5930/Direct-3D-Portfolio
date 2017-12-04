#include "Layer.h"
#include "GameObject.h"

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
:m_pDevice(pDevice)
{

}

Engine::CLayer::~CLayer(void)
{
	Release();
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

HRESULT Engine::CLayer::AddObject(const TCHAR* pObjKey, CGameObject* pGameObject)
{
	if(pGameObject != NULL)
	{
		map<const TCHAR*, list<CGameObject*>>::iterator iter = m_mapObjList.begin();
		map<const TCHAR*, list<CGameObject*>>::iterator iter_end = m_mapObjList.end();

		map<const TCHAR*, list<CGameObject*>>::iterator find_iter = find_if(iter, iter_end, CTagFinder(pObjKey));
		
		if(find_iter == iter_end)
		{
			m_mapObjList[pObjKey] = list<CGameObject*>();
		}

		m_mapObjList[pObjKey].push_back(pGameObject);
	}

	return S_OK;
}

int Engine::CLayer::Update(void)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = m_mapObjList.begin();
	map<const TCHAR*, list<CGameObject*>>::iterator	iter_end = m_mapObjList.end();

	for (iter; iter != iter_end; ++iter)
	{
		list<CGameObject*>::iterator	iterList = iter->second.begin();
		list<CGameObject*>::iterator	iterList_end = iter->second.end();

		for (iterList; iterList != iterList_end;)		
		{
			if((*iterList)->Update() == 1)
			{
				delete (*iterList);
				iterList = iter->second.erase(iterList);
			}
			else
				++iterList;
		}		
	}	
	
	return 0;
}

void Engine::CLayer::Render(void)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = m_mapObjList.begin();
	map<const TCHAR*, list<CGameObject*>>::iterator	iter_end = m_mapObjList.end();

	for (iter; iter != iter_end; ++iter)
	{
		list<CGameObject*>::iterator	iterList = iter->second.begin();
		list<CGameObject*>::iterator	iterList_end = iter->second.end();

		for (iterList; iterList != iterList_end; ++iterList)		
		{
			(*iterList)->Render();
		}		
	}	
}

void Engine::CLayer::Release(void)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = m_mapObjList.begin();
	map<const TCHAR*, list<CGameObject*>>::iterator	iter_end = m_mapObjList.end();

	for (; iter != iter_end; ++iter)
	{
		list<CGameObject*>::iterator	iterList = iter->second.begin();
		list<CGameObject*>::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
			Engine::Safe_Delete((*iterList));

		iter->second.clear();
	}
	m_mapObjList.clear();
}

Engine::CComponent* Engine::CLayer::GetComponent(TCHAR* pObjKey, TCHAR* pComponentKey)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = m_mapObjList.begin();
	map<const TCHAR*, list<CGameObject*>>::iterator	iter_end = m_mapObjList.end();

	map<const TCHAR*, list<CGameObject*>>::iterator find_iter = find_if(iter, iter_end, CTagFinder(pObjKey));
	
	if(find_iter == iter_end)
		return NULL;

	list<CGameObject*>::iterator	iterlist = find_iter->second.begin();
	list<CGameObject*>::iterator	iterlist_end = find_iter->second.end();

	for( ;iterlist != iterlist_end ; ++iterlist)
	{
		CComponent* pComponent = (*iterlist)->GetComponent(pComponentKey);

		if(pComponentKey != NULL)
			return pComponent;
	}

	return NULL;
}

Engine::CGameObject* Engine::CLayer::GetGameObject(TCHAR* pObjKey)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = m_mapObjList.begin();
	map<const TCHAR*, list<CGameObject*>>::iterator	iter_end = m_mapObjList.end();

	map<const TCHAR*, list<CGameObject*>>::iterator find_iter = find_if(iter, iter_end, CTagFinder(pObjKey));

	if(find_iter == iter_end)
		return NULL;

	return find_iter->second.front();
}