#include "stdafx.h"
#include "LogoBack.h"
#include "Export_Function.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
:Engine::CGameObject(pDevice), m_pResourceMgr(Engine::Get_ResourceMgr())
{
	ZeroMemory(&m_MtrInfo, sizeof(D3DMATERIAL9));
}

CLogoBack::~CLogoBack(void)
{
	Release();
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack*	pGameObject = new CLogoBack(pDevice);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	
	m_MtrInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	return S_OK;
}

HRESULT CLogoBack::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(D3DXVECTOR3(0.f, 0.f, -1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResources(Resource_Static, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResources(Resource_Logo, L"Texture_Logo");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	return S_OK;
}

int CLogoBack::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}	

void CLogoBack::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pDevice->SetMaterial(&m_MtrInfo);
	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CLogoBack::Release(void)
{

}