#include "stdafx.h"
#include "Sphere.h"
#include "Transform.h"

CSphere::CSphere(LPDIRECT3DDEVICE9 pDevice)
:Engine::CGameObject(pDevice), m_pInfo(NULL), m_pMesh(NULL)
{

}

CSphere::~CSphere(void)
{
	Release();
}

HRESULT CSphere::Initialize(D3DXVECTOR3 vPos)
{	
	m_pInfo = Engine::CTransform::Create(D3DXVECTOR3(0.f, 1.f, 0.f));
	
	m_pInfo->m_vPos = vPos;

	HRESULT hr = NULL;

	hr = D3DXCreateSphere(m_pDevice, 1.f, 10, 10, &m_pMesh, 0);

	FAILED_CHECK(hr);

	return S_OK;
}

void CSphere::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pMesh->DrawSubset(0);
}

int CSphere::Update(void)
{
	m_pInfo->Update();
	
	return 0;
}

void CSphere::Release(void)
{
	Engine::Safe_Delete(m_pInfo);
	Engine::Safe_Release(m_pMesh);
	Engine::Safe_Delete(m_pMesh);
}