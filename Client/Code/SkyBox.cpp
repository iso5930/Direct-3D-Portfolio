#include "stdafx.h"
#include "Include.h"
#include "SkyBox.h"
#include "GameObject.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "CubeTex.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice)
{

}

CSkyBox::~CSkyBox(void)
{
	Release();
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkyBox* pGameObject = new CSkyBox(pDevice);

	if(FAILED(pGameObject->Initialize()))
	{
		Engine::Safe_Delete(pGameObject);
	}

	return pGameObject;
}

HRESULT CSkyBox::Initialize(void)
{	
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(100.f, 100.f, 100.f);

	return S_OK;
}

int CSkyBox::Update(void)
{	
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(D3DXVECTOR3));

	m_pInfo->m_vPos = vCamPos;

	Engine::CGameObject::Update();

	return 0;
}

void CSkyBox::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pTexture->Render(0, 1);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CSkyBox::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Stage, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Stage, L"Texture_SkyBox");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	return S_OK;
}

void CSkyBox::Release(void)
{

}