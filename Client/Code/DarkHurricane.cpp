#include "stdafx.h"
#include "Include.h"
#include "DarkHurricane.h"
#include "Export_Function.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Collision.h"
#include "Shader.h"

CDarkHurricane::CDarkHurricane(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice), m_pInfo(NULL), m_pMesh(NULL), m_pCollision(NULL), m_pShader(NULL)
, m_fAccTime(0.f), m_fMaxTime(0.f)
{

}

CDarkHurricane::~CDarkHurricane(void)
{
	Engine::Safe_Delete(m_pCollision);
}

CDarkHurricane* CDarkHurricane::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDarkHurricane* pManaAmber = new CDarkHurricane(pDevice);

	if(FAILED(pManaAmber->Initialize()))
	{
		Engine::Safe_Delete(pManaAmber);
	}

	return pManaAmber;
}

HRESULT CDarkHurricane::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	D3DXMatrixIdentity(&m_matCombine);
	m_pCollision = Engine::CCollision::Create(m_pDevice, 45.f, &m_pInfo->m_matWorld, &m_matCombine);
	m_pCollision->SetScale(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pCollision->Update();

	m_eType = Skill_DarkHurricane;

	m_pInfo->m_vScale = D3DXVECTOR3(0.7f, 0.7f, 0.7f);
	m_pInfo2->m_vScale = D3DXVECTOR3(0.7f, 0.7f, 0.7f);

	m_pInfo2->m_fAngle[Angle_Y] = D3DXToRadian(180.f);

	m_fMaxTime = 3.f;

	return S_OK;
}

void CDarkHurricane::Render(void)
{	
	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );
	m_pMesh->RenderMesh(pEffect, NULL);

	hr = SetConstantTable2();
	FAILED_CHECK_RETURN(hr, );
	m_pMesh2->RenderMesh(pEffect, NULL);
// 
// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 
// 	m_pCollision->SetBOOL(false, true);
// 	m_pCollision->Render();
// 
// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

int CDarkHurricane::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime;

	if(m_fAccTime >= m_fMaxTime)
	{
		return 1;
	}

	m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * 200.f;
	m_pInfo2->m_vPos += m_pInfo->m_vDir * fTime * 200.f;

	m_pInfo->m_fAngle[Angle_Y] += D3DXToRadian(720.f) * fTime;
	m_pInfo2->m_fAngle[Angle_Y] += D3DXToRadian(720.f) * fTime;

	Engine::CGameObject::Update();

	m_pCollision->Update();

	return 0;
}

HRESULT CDarkHurricane::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Transform
	pComponent = m_pInfo2 = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform2", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"DarkHurricane");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Mesh", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"DarkHurricane");
	m_pMesh2 = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh2, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Mesh2", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_MagicArrow");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pShader = (Engine::CShader*)pComponent;
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CDarkHurricane::SetConstantTable(void)
{
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	FAILED_CHECK_RETURN(pEffect, E_FAIL);

	D3DXMATRIX matView, matProj, matInverWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInverWorld, NULL, &m_pInfo->m_matWorld);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetMatrix("g_matInverWorld", &matInverWorld);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffus", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3 vCameraPos(matView._41, matView._42, matView._43);

	pEffect->SetVector("g_vCameraPos", &D3DXVECTOR4(vCameraPos, 0.f));

	return S_OK;
}

void CDarkHurricane::SetPos(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

Engine::CCollision* CDarkHurricane::GetCollision(void)
{
	return m_pCollision;
}

void CDarkHurricane::SetAngle(float fAngle)
{
	m_pInfo->m_fAngle[Angle_Y] = fAngle;
}

HRESULT CDarkHurricane::SetConstantTable2(void)
{
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	FAILED_CHECK_RETURN(pEffect, E_FAIL);

	D3DXMATRIX matView, matProj, matInverWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInverWorld, NULL, &m_pInfo2->m_matWorld);

	pEffect->SetMatrix("g_matWorld", &m_pInfo2->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetMatrix("g_matInverWorld", &matInverWorld);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffus", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3 vCameraPos(matView._41, matView._42, matView._43);

	pEffect->SetVector("g_vCameraPos", &D3DXVECTOR4(vCameraPos, 0.f));

	return S_OK;
}

void CDarkHurricane::SetDir(D3DXVECTOR3 vDir)
{
	m_pInfo->m_vDir = vDir;
}