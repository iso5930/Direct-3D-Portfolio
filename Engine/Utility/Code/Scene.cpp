#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CScene::~CScene(void)
{
	Release();
}

void Engine::CScene::Update(void)
{
	map<WORD, CLayer*>::iterator iter		= m_mapLayer.begin();
	map<WORD, CLayer*>::iterator iter_end	= m_mapLayer.end();	

 	for(iter; iter != iter_end; ++iter)
 	{
 		iter->second->Update();
 	}	
}

void Engine::CScene::Render(void)
{
	map<WORD, CLayer*>::iterator iter		= m_mapLayer.begin();
	map<WORD, CLayer*>::iterator iter_end	= m_mapLayer.end();

	for(iter; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}	
}

void Engine::CScene::Release(void)
{
	map<WORD, CLayer*>::iterator iter		= m_mapLayer.begin();
	map<WORD, CLayer*>::iterator iter_end	= m_mapLayer.end();

	for_each(iter, iter_end, CDeleteMap());
	m_mapLayer.clear();
}

Engine::CComponent* Engine::CScene::GetComponent(WORD LayerID, TCHAR* pObjKey, TCHAR* pComponentKey)
{
	map<WORD, CLayer*>::iterator iter = m_mapLayer.find(LayerID);

	if(iter == m_mapLayer.end())
	{
		return NULL;
	}

	return iter->second->GetComponent(pObjKey, pComponentKey);
}

Engine::CGameObject* Engine::CScene::GetGameObject(WORD LayerID, TCHAR* pObjKey)
{
	map<WORD, CLayer*>::iterator iter = m_mapLayer.find(LayerID);

	if(iter == m_mapLayer.end())
	{
		return NULL;
	}

	return iter->second->GetGameObject(pObjKey);
}