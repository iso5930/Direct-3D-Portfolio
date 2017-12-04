#include "stdafx.h"
#include "Include.h"
#include "DarkBomb.h"
#include "Export_Function.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Shader.h"

CDarkBomb::CDarkBomb(LPDIRECT3DDEVICE9 pDevice)
:CEffect(pDevice), m_pMesh(NULL), m_pShader(NULL), m_fAccTime(0.f), m_fMaxTime(0.f)
{

}

CDarkBomb::~CDarkBomb(void)
{

}

CDarkBomb* CDarkBomb::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDarkBomb* pEffect = new CDarkBomb(pDevice);

	if(FAILED(pEffect->Initialize()))
	{
		Engine::Safe_Delete(pEffect);
	}

	return pEffect;
}

HRESULT CDarkBomb::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_fMaxTime = 1.f;

	m_pInfo->m_vScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);

	return S_OK;
}

int CDarkBomb::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime;

	if(m_fAccTime >= fTime)
	{
		return 1;
	}

	CGameObject::Update();

	return 0;
}

void CDarkBomb::Render(void)
{
	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh(pEffect, NULL);
}	

HRESULT CDarkBomb::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Dark_Bomb");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Mesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_MagicArrow");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pShader = (Engine::CShader*)pComponent;
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CDarkBomb::SetConstantTable(void)
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

void CDarkBomb::SetPos(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}
