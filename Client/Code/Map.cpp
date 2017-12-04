#include "stdafx.h"
#include "Include.h"
#include "Map.h"
#include "Export_Function.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Shader.h"

CMap::CMap(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice), m_pInfo(NULL), m_pResourceMgr(NULL), m_pMesh(NULL)
, m_pShader(NULL)
{

}

CMap::~CMap(void)
{
	Release();
}

CMap* CMap::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMap* pMap = new CMap(pDevice);

	if(FAILED(pMap->Initialize()))
	{
		Engine::Safe_Delete(pMap);
	}

	return pMap;
}

HRESULT CMap::Initialize(void)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();

	FAILED_CHECK(AddComponent());

	return S_OK;
}

int CMap::Update(void)
{
	CGameObject::Update();

	return 0;
}

void CMap::Render(void)
{
	SetConstantTable();
	
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh(pEffect, NULL);
}

HRESULT CMap::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResources(Resource_Stage, L"1Stage_Map");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"StaticMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Map");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));
	
	return S_OK;
}

void CMap::Release(void)
{

}

HRESULT CMap::SetConstantTable(void)
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

	pEffect->SetVector("vCameraPos", &D3DXVECTOR4(vCameraPos, 0.f));

	Engine::Get_ShadowMgr()->SetShadowEffect(pEffect, g_vLightPos);
	
	return S_OK;
}

Engine::CStaticMesh* CMap::GetMesh(void)
{
	return m_pMesh;
}