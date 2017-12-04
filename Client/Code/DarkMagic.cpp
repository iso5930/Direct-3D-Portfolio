#include "stdafx.h"
#include "Include.h"
#include "DarkMagic.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer.h"

CDarkMagic::CDarkMagic(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice)
{

}

CDarkMagic::~CDarkMagic(void)
{

}

CDarkMagic* CDarkMagic::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDarkMagic* pSkill = new CDarkMagic(pDevice);

	if(FAILED(pSkill->Initialize()))
	{
		Safe_Delete(pSkill);
	}

	return pSkill;
}

HRESULT CDarkMagic::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pInfo->m_vScale = D3DXVECTOR3(60.f, 60.f, 60.f);
	m_pInfo->m_fAngle[Angle_X] = D3DXToRadian(90.f);
	m_pInfo->m_fAngle[Angle_Y] = 0.f;

	m_fTime = 1.5f;

	m_fAccTime = 0.f;

	m_eType = Skill_DarkBolt;

	m_fRadius = 60.f;

	return S_OK;
}

void CDarkMagic::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

int CDarkMagic::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime;

	if(m_fTime <= m_fAccTime)
	{
		return 1;
	}

	CGameObject::Update();

	return 0;
}

HRESULT CDarkMagic::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"DarkMagic");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	return S_OK;
}

Engine::CTransform* CDarkMagic::GetInfo(void)
{
	return m_pInfo;
}