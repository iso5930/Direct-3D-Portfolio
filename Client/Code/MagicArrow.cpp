#include "stdafx.h"
#include "Include.h"
#include "Export_Function.h"
#include "MagicArrow.h"
#include "Transform.h"
#include "RcTex.h"
#include "Texture.h"
#include "Shader.h"
#include "Effect.h"
#include "EffectMgr.h"
#include "Collision.h"
#include "StaticMesh.h"
#include "MagicArrowEffect.h"

CMagicArrow::CMagicArrow(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice), m_pInfo(NULL), m_fTime(0.f), m_fAccTime(0.f), m_pBuffer(NULL), m_pTexture(NULL)
, m_pEffect(NULL)
{

}

CMagicArrow::~CMagicArrow(void)
{
	Safe_Delete(m_pCollision);
	Safe_Delete(m_pEffect);
}

CMagicArrow* CMagicArrow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMagicArrow* pSkill = new CMagicArrow(pDevice);

	if(FAILED(pSkill->Initialize()))
	{
		Engine::Safe_Delete(pSkill);
	}

	return pSkill;
}

HRESULT CMagicArrow::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pInfo->m_vScale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);

	//자체적으로 이펙트도 생성을 해야한다.
	//그리고 

	//다른 물체에 닿으면 바로 지워줘야 한다.
	D3DXMatrixIdentity(&m_matCombine);

	m_pCollision = Engine::CCollision::Create(m_pDevice, 25.f, &m_pInfo->m_matWorld, &m_matCombine);
	m_pCollision->SetScale(D3DXVECTOR3(25.f, 25.f, 25.f));
	m_pCollision->Update();

	m_eType = Skill_MagicArrow;

	m_pEffect = CMagicArrowEffect::Create(m_pDevice);
	
	return S_OK;
}

int CMagicArrow::Update(void)
{
	if(m_bDeath)
	{
		return 1;
	}

	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime;

	if(m_fTime <= m_fAccTime)
	{
		return 1;
	}

	m_pInfo->m_fAngle[Angle_Y] += D3DX_PI * fTime * 5.f;
	m_pInfo->m_fAngle[Angle_Z] += D3DX_PI * fTime * 5.f;
	m_pInfo->m_fAngle[Angle_X] += D3DX_PI * fTime * 5.f;

	m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * 200.f;

	m_pCollision->Update();

	//cout << "매직 에로우 업데이트 " << endl;
	
	Engine::CGameObject::Update();

	m_pEffect->SetDir(m_pInfo->m_vDir);
	m_pEffect->SetInfo(m_pInfo->m_vPos);
	m_pEffect->Update();
	
	return 0;
}

void CMagicArrow::Render(void)
{
	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

 	//m_pTexture->Render(0, 0);
 	//m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pMesh->RenderMesh(pEffect, NULL);

	m_pEffect->Render();

// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 
// 	m_pCollision->SetBOOL(true, false);
// 	m_pCollision->Render();
// 
// 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CMagicArrow::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	////Buffer
	//pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Buffer_RcTex");
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pBuffer = (Engine::CRcTex*)pComponent;
	//m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Buffer", pComponent));

	////Texture
	//pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Magic_Arrow");
	//NULL_CHECK_RETURN(pComponent,  E_FAIL);
	//m_pTexture = (Engine::CTexture*)pComponent;
	//m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"MagicArrow_Mesh");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pMesh = (Engine::CStaticMesh*)pComponent;
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Mesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_MagicArrow");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pShader = (Engine::CShader*)pComponent;
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));
	
	return S_OK;
}	

void CMagicArrow::SetInfo(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CMagicArrow::SetTime(float fTime)
{
	m_fTime = fTime;
}

void CMagicArrow::SetDir(D3DXVECTOR3 vDir)
{
	m_pInfo->m_vDir = vDir;
}

void CMagicArrow::SetAngle(float fAngle)
{
	m_pInfo->m_fAngle[Angle_Y] = fAngle;
}

HRESULT CMagicArrow::SetConstantTable(void)
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

Engine::CCollision* CMagicArrow::GetCollision(void)
{
	return m_pCollision;
}