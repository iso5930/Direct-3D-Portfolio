#include "stdafx.h"
#include "Include.h"
#include "DarkBomb2.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Export_Function.h"

CDarkBomb2::CDarkBomb2(LPDIRECT3DDEVICE9 pDevice)
:CEffect(pDevice), m_fEndFrame(0.f), m_fCurFrame(0.f), m_fSpeed(0.f)
{

}

CDarkBomb2::~CDarkBomb2(void)
{

}


CDarkBomb2* CDarkBomb2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDarkBomb2* pEffect = new CDarkBomb2(pDevice);

	if(FAILED(pEffect->Initialize()))
	{
		Engine::Safe_Delete(pEffect);
	}

	return pEffect;
}

HRESULT CDarkBomb2::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_fEndFrame = 11.f;

	m_pInfo->m_vScale = D3DXVECTOR3(50.f, 50.f, 50.f);

	m_fSpeed = 20.f;

	return S_OK;
}		

int CDarkBomb2::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fCurFrame += fTime * m_fSpeed;

	D3DXMATRIX matBill;
	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	CGameObject::Update();

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	if(m_fCurFrame >= m_fEndFrame)
	{
		return 1;
	}

	return 0;
}

void CDarkBomb2::Render(void)
{
	m_pTexture->Render(0, (DWORD)m_fCurFrame);
	m_pBuffer->Render(&m_pInfo->m_matWorld);
}

HRESULT CDarkBomb2::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"DarkFire_Bomb");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	return S_OK;
}

void CDarkBomb2::SetInfo(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}