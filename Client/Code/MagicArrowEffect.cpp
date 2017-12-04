#include "stdafx.h"
#include "MagicArrowEffect.h"
#include "Shader.h"
#include "Export_Function.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Include.h"

CMagicArrowEffect::CMagicArrowEffect(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pMesh(NULL), m_pInfo(NULL), m_pShader(NULL), m_fAngle(0.f)
{

}

CMagicArrowEffect::~CMagicArrowEffect(void)
{

}

CMagicArrowEffect* CMagicArrowEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMagicArrowEffect* pEffect = new CMagicArrowEffect(pDevice);

	if(FAILED(pEffect->Initialize()))
	{
		Engine::Safe_Delete(pEffect);
	}

	return pEffect;
}

HRESULT CMagicArrowEffect::Initialize(void)
{
	HRESULT hr = AddComponent();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}

int CMagicArrowEffect::Update(void)
{
 	float fTime = Engine::Get_TimeMgr()->GetTime();

	float fAngle = D3DXVec3Dot(&m_pInfo->m_vDir, &g_vLook);
	fAngle = acosf(fAngle);

	float fAngleX = D3DXVec3Dot(&m_pInfo->m_vDir, &D3DXVECTOR3(1.f, 0.f, 0.f));
	fAngleX = acosf(fAngleX);

	if(D3DXToDegree(fAngleX) >= 90.f)
	{
		fAngle = 2 * D3DX_PI - fAngle;
	}

	m_fAngle += D3DXToRadian(150.f) * fTime;

	m_pInfo->m_fAngle[Angle_Y] = fAngle;

	D3DXMATRIX matRotAxis, matTrans, matRotY;
	D3DXMatrixRotationAxis(&matRotAxis, &m_pInfo->m_vDir, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);
	D3DXMatrixRotationY(&matRotY, m_pInfo->m_fAngle[Angle_Y]);
	//Engine::CGameObject::Update();

	m_pInfo->m_matWorld = matRotY * matRotAxis * matTrans;

	return 0;
}

void CMagicArrowEffect::Render(void)
{
	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	m_pMesh->RenderMesh(pEffect, NULL);
}

HRESULT CMagicArrowEffect::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"MagicArrow_Effect");
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

HRESULT CMagicArrowEffect::SetConstantTable(void)
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

void CMagicArrowEffect::SetInfo(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CMagicArrowEffect::SetAngle(float fAngle)
{
	m_pInfo->m_fAngle[Angle_Y] = fAngle;
}

void CMagicArrowEffect::SetDir(D3DXVECTOR3 vDir)
{
	m_pInfo->m_vDir = vDir;
}	