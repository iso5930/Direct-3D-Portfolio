#include "Management.h"
#include "Scene.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
:m_pScene(NULL), m_pRenderer(NULL), m_pDevice(NULL)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	m_pRenderer = CRenderer::Create(pDevice);

	if(m_pRenderer == NULL)
	{
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if(m_pScene != NULL)
	{
		m_pScene->Update();
	}

	Engine::Get_SkillMgr()->Update();
}

void Engine::CManagement::Render(const float& fTime)
{
	if(m_pRenderer != NULL)
	{
		m_pRenderer->Render(fTime);
	}
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Delete(m_pRenderer);
	Engine::Safe_Delete(m_pScene);
}

Engine::CComponent* Engine::CManagement::GetComponent(WORD LayerID, TCHAR* pObjKey, TCHAR* pComponentKey)
{
	if(m_pScene == NULL)
	{
		return NULL;
	}

	return m_pScene->GetComponent(LayerID, pObjKey, pComponentKey);
}

void Engine::CManagement::Add_RenderGroup(eRenderType _Type, CGameObject* pObj)
{	
	m_pRenderer->Add_RenderGroup(pObj, _Type);
}

void Engine::CManagement::RenderGroupRelease(void)
{
	m_pRenderer->Release();
}

Engine::CGameObject* Engine::CManagement::GetGameObject(WORD LayerID, TCHAR* pObjKey)
{
	if(m_pScene == NULL)
	{
		return NULL;
	}

	return m_pScene->GetGameObject(LayerID, pObjKey);
}