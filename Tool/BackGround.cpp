#include "stdafx.h"
#include "BackGround.h"
#include "Export_Function.h"
#include "Transform.h"
#include "StaticMesh.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pInfo(NULL), m_pMesh(NULL), m_bFrame(true)
{
	
}

CBackGround::~CBackGround(void)
{
	Engine::Safe_Delete(m_pInfo);
	Engine::Safe_Delete(m_pMesh);
}

HRESULT CBackGround::Initialize(void)
{
	Engine::CComponent* pComponent = NULL;

	pComponent = m_pInfo = Engine::CTransform::Create(D3DXVECTOR3(0.f, 0.f, -1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));
	
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"1Stage_Map");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	//m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"StaticMesh", pComponent));

	return S_OK;
}

int CBackGround::Update(void)
{
	m_pInfo->Update();
	m_pMesh->Update();

	return 0;
}

void CBackGround::Render(void)
{
	m_pMesh->RenderMesh(NULL, &m_pInfo->m_matWorld);
}