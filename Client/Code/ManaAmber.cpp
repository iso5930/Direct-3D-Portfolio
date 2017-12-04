#include "stdafx.h"
#include "Include.h"
#include "ManaAmber.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Export_Function.h"
#include "Shader.h"
#include "Collision.h"

CManaAmber::CManaAmber(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pInfo(NULL), m_pMesh(NULL), m_pCollision(NULL), m_pShader(NULL)
{

}

CManaAmber::~CManaAmber(void)
{

}

HRESULT CManaAmber::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	D3DXMatrixIdentity(&m_matCombine);

	m_pCollision = Engine::CCollision::Create(m_pDevice, 50.f, &m_pInfo->m_matWorld, &m_matCombine);

	return S_OK;
}

void CManaAmber::Render(void)
{
	if(m_bView == false)
	{
		return;
	}

	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh(pEffect, NULL);
}

int CManaAmber::Update(void)
{
	Engine::CGameObject::Update();

	m_pCollision->Update();

	return 0;
}

HRESULT CManaAmber::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"ManaAmber");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Mesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_ManaAmber");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pShader = (Engine::CShader*)pComponent;
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));

	return S_OK;
}

void CManaAmber::SetPos(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

CManaAmber* CManaAmber::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CManaAmber* pManaAmber = new CManaAmber(pDevice);

	if(FAILED(pManaAmber->Initialize()))
	{
		Engine::Safe_Delete(pManaAmber);
	}

	return pManaAmber;
}

HRESULT CManaAmber::SetConstantTable(void)
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

void CManaAmber::SetView(bool _View)
{
	m_bView = _View;
}