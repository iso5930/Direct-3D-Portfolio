#include "stdafx.h"
#include "Include.h"
#include "FireShock.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer.h"


CFireShock::CFireShock(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice)
{
	
}

CFireShock::~CFireShock(void)
{
	
}

CFireShock* CFireShock::Create(LPDIRECT3DDEVICE9 pDevice)
{	
	CFireShock* pSkill = new CFireShock(pDevice);

	if(FAILED(pSkill->Initialize()))
	{
		Safe_Delete(pSkill);
	}

	return pSkill;
}

HRESULT CFireShock::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pInfo->m_vScale = D3DXVECTOR3(60.f, 60.f, 60.f);
	m_pInfo->m_fAngle[Angle_X] = D3DXToRadian(90.f);
	m_pInfo->m_fAngle[Angle_Y] = 0.f;

	m_fTime = 4.f;

	m_fAccTime = 0.f;

	m_eType = Skill_FireShock;

	m_fRadius = 60.f;
	
	return S_OK;
}

int CFireShock::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime;

	m_pInfo->m_fAngle[Angle_Y] += D3DXToRadian(90.f) * fTime;

  	if(m_fTime <= m_fAccTime)
  	{
  		return 1;
  	}

	CGameObject::Update();

	return 0;
}

void CFireShock::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

 	m_pTexture->Render(0, 3);
 	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pTexture->Render(0, 4);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pTexture->Render(0, 7);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFireShock::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Fire");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Fire_Texture", pComponent));

	return S_OK;
}

Engine::CTransform* CFireShock::GetInfo(void)
{
	return m_pInfo;
}