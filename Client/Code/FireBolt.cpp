#include "stdafx.h"
#include "Include.h"
#include "FireBolt.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "EffectMgr.h"
#include "FireBomb.h"
#include "Collision.h"
#include "SoundMgr.h"

CFireBolt::CFireBolt(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice), m_fTime(0.f), m_fAccTime(0.f), m_pTexture(NULL), m_pBuffer(NULL), m_pInfo(NULL), m_fCurFrame(0.f), m_fEndFrame(0.f)
, m_pCollision(NULL)
{

}

CFireBolt::~CFireBolt(void)
{
	Release();
}

CFireBolt* CFireBolt::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFireBolt* pSkill = new CFireBolt(pDevice);

	if(FAILED(pSkill->Initialize()))
	{
		Engine::Safe_Delete(pSkill);
	}

	return pSkill;
}

HRESULT CFireBolt::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pInfo->m_vScale = D3DXVECTOR3(30.f, 30.f, 30.f);
	m_fTime = 8.f;

	m_fEndFrame = 8.f;

	m_eType = Skill_FireBolt;

	m_fRadius = 30.f;

	D3DXMatrixIdentity(&m_matCombine);

	m_pCollision = Engine::CCollision::Create(m_pDevice, 30.f, &m_pInfo->m_matWorld, &m_matCombine);
	m_pCollision->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	return S_OK;
}

void CFireBolt::Render(void)
{
	((Engine::CTexture*)GetComponent(L"FireBolt"))->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pTexture->Render(0, (DWORD)m_fCurFrame);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 
// 	m_pCollision->SetBOOL(true, false);
// 	m_pCollision->Render();
// 
// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

int CFireBolt::Update(void)
{
	if(m_bDeath)
	{
		return 1;
	}

	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime * 10.f;

	if(m_fAccTime >= m_fTime)
	{
		return 1;
	}
	
	m_pInfo->m_vPos += m_pInfo->m_vDir * 200.f * fTime;

	m_fCurFrame += fTime * 10.f;
	
	if(m_fCurFrame >= m_fEndFrame)
	{
		m_fCurFrame = 0.f;
	}

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

	Engine::CGameObject::Update();

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	m_pCollision->Update();

	return 0;
}

HRESULT CFireBolt::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"FireBall_Ring");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Fire2");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"FireBolt", pComponent));

	return S_OK;
}

Engine::CTransform* CFireBolt::GetInfo(void)
{
	return m_pInfo;
}

void CFireBolt::Release(void)
{
	CFireBomb* pEffect = CFireBomb::Create(m_pDevice);
	pEffect->SetInfo(m_pInfo->m_vPos);

	CEffectMgr::GetInstance()->AddEffect(pEffect);

	Engine::Safe_Delete(m_pCollision);

	CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_dashfireball_crash.wav");	
}

Engine::CCollision* CFireBolt::GetCollision(void)
{
	return m_pCollision;
}