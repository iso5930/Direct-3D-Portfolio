#include "stdafx.h"
#include "Include.h"
#include "IceBlast.h"
#include "Export_Function.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Collision.h"
#include "IceBomb.h"
#include "EffectMgr.h"

CIceBlast::CIceBlast(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice), m_fAccTime(0.f), m_fTime(0.f), m_pMesh(NULL)
{

}

CIceBlast::~CIceBlast(void)
{
	Release();
}

CIceBlast* CIceBlast::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CIceBlast* pSkill = new CIceBlast(pDevice);

	if(FAILED(pSkill->Initialize()))
	{
		Engine::Safe_Delete(pSkill);
	}

	return pSkill;
}

HRESULT CIceBlast::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);
	m_fAngle = 0.f;
	
	m_fTime = 1.f;

	m_eType = Skill_IceBlast;

	D3DXMatrixIdentity(&m_matCombine);

	m_pCollision = Engine::CCollision::Create(m_pDevice, 35.f, &m_pInfo->m_matWorld, &m_matCombine);
	m_pCollision->SetScale(D3DXVECTOR3(12.f, 12.f, 60.f));
	m_pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 30.f));

	return S_OK;
}

void CIceBlast::Render(void)
{
	m_pMesh->RenderMesh(NULL, &m_pInfo->m_matWorld);

// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 
// 	m_pCollision->SetBOOL(true, true);
// 	m_pCollision->Render();
// 
// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

int CIceBlast::Update(void)
{
	if(m_bDeath)
	{
		return 1;
	}

	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * 200.f;

	m_fAngle += D3DXToRadian(45.f) * fTime * 10.f;
	m_fAccTime += fTime;

	if(m_fAccTime >= m_fTime)
	{
		return 1;
	}
	
	D3DXMATRIX matRot, matTrans, matRotY;
	D3DXMatrixRotationAxis(&matRot, &m_pInfo->m_vDir, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);
	D3DXMatrixRotationY(&matRotY, m_pInfo->m_fAngle[Angle_Y]);
	
	m_pInfo->m_matWorld = matRotY * matRot * matTrans;

	m_pCollision->Update();

	return 0;
}

HRESULT CIceBlast::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"IceBlast_Mesh");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Mesh", pComponent));

	return S_OK;
}

Engine::CTransform* CIceBlast::GetInfo(void)
{
	return m_pInfo;
}

Engine::CCollision* CIceBlast::GetCollision(void)
{	
	return m_pCollision;
}

void CIceBlast::Release(void)
{
	CIceBomb* pEffect = CIceBomb::Create(m_pDevice);
	pEffect->SetInfo(m_pInfo->m_vPos);

	CEffectMgr::GetInstance()->AddEffect(pEffect);

	Engine::Safe_Delete(m_pCollision);
}