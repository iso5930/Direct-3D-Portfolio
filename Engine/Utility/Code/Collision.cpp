#include "Collision.h"
#include "Transform.h"
#include "Export_Function.h"
#include "CubeColor.h"

Engine::CCollision::CCollision(LPDIRECT3DDEVICE9 pDevice)
:m_pDevice(pDevice), m_pInfo(NULL), m_pMesh(NULL), m_pBuffer(NULL), m_pmatCombined(NULL), m_pmatInfo(NULL)
{
	ZeroMemory(&m_OBB, sizeof(OBB));
}

Engine::CCollision::~CCollision(void)
{
	Release();
}

Engine::CCollision* Engine::CCollision::Create(LPDIRECT3DDEVICE9 pDevice, float fRadius, const D3DXMATRIX* pmatInfo, const D3DXMATRIX* pmatCombined)
{
	CCollision* pCollision = new CCollision(pDevice);

	if(FAILED(pCollision->Initialize(fRadius, pmatInfo, pmatCombined)))
	{
		Engine::Safe_Delete(pCollision);
	}

	return pCollision;
}

HRESULT Engine::CCollision::Initialize(float fRadius, const D3DXMATRIX* pmatInfo, const D3DXMATRIX* pmatCombined)
{
	m_pmatCombined	= pmatCombined;
	m_pmatInfo		= pmatInfo;

	D3DXVECTOR3 vMin(-0.5, -0.5f, -0.5f);
	D3DXVECTOR3 vMax(0.5, 0.5f, 0.5f);

	InitPoint(&m_OBB, &vMin, &vMax);
	
	m_pInfo = Engine::CTransform::Create(D3DXVECTOR3(0.f, 0.f, 1.f));

	m_pInfo->Update();

	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatCombined) * (*m_pmatInfo);

	HRESULT hr = D3DXCreateSphere(m_pDevice, fRadius, 10, 10, &m_pMesh, 0);
	FAILED_CHECK(hr);

	m_fRadius = fRadius;
	
	m_pBuffer = CCubeColor::Create(m_pDevice, vMin, vMax, D3DCOLOR_ARGB(255, 255, 0, 0));
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	
	return S_OK;
}

void Engine::CCollision::InitPoint(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	pOBB->vPoint[0] = D3DXVECTOR3(pMin->x, pMax->y, pMin->z);
	pOBB->vPoint[1] = D3DXVECTOR3(pMax->x, pMax->y, pMin->z);
	pOBB->vPoint[2] = D3DXVECTOR3(pMax->x, pMin->y, pMin->z);
	pOBB->vPoint[3] = D3DXVECTOR3(pMin->x, pMin->y, pMin->z);

	pOBB->vPoint[4] = D3DXVECTOR3(pMin->x, pMax->y, pMax->z);
	pOBB->vPoint[5] = D3DXVECTOR3(pMax->x, pMax->y, pMax->z);
	pOBB->vPoint[6] = D3DXVECTOR3(pMax->x, pMin->y, pMax->z);
	pOBB->vPoint[7] = D3DXVECTOR3(pMin->x, pMin->y, pMax->z);

	for(int i = 0; i < 8; ++i)
		pOBB->vCenter += pOBB->vPoint[i];

	pOBB->vCenter *= 0.125f;
}

void Engine::CCollision::InitAxis(OBB* pOBB)
{
	//x
	pOBB->vProjAxis[0] = (pOBB->vPoint[1] + pOBB->vPoint[2] + pOBB->vPoint[5] + pOBB->vPoint[6]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[0] = pOBB->vPoint[1] - pOBB->vPoint[0];

	//y
	pOBB->vProjAxis[1] = (pOBB->vPoint[0] + pOBB->vPoint[1] + pOBB->vPoint[4] + pOBB->vPoint[5]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];

	//z
	pOBB->vProjAxis[2] = (pOBB->vPoint[4] + pOBB->vPoint[5] + pOBB->vPoint[6] + pOBB->vPoint[7]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for(int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pOBB->vAxis[i], &pOBB->vAxis[i]);
}

int Engine::CCollision::Update(void)
{
	m_pInfo->Update();

	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatCombined) * (*m_pmatInfo);
	
	D3DXVECTOR3 vPos(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	D3DXMatrixTranslation(&m_matWorld, vPos.x, vPos.y, vPos.z);

	m_matWorld = m_matWorld * (*m_pmatCombined) * (*m_pmatInfo);

	return 0;
}

void Engine::CCollision::Render(void)
{
	if(m_bSphere)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		m_pMesh->DrawSubset(0);
	}

	if(m_bOBB)
	{
		m_pDevice->SetTexture(0, NULL);
		m_pBuffer->Render(&m_pInfo->m_matWorld);
	}
}

void Engine::CCollision::Release(void)
{
	Safe_Delete(m_pInfo);
	Safe_Release(m_pMesh);
	Safe_Delete(m_pMesh);
	Safe_Delete(m_pBuffer);
}

void Engine::CCollision::SetPos(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

void Engine::CCollision::SetScale(D3DXVECTOR3 vScale)
{
	m_pInfo->m_vScale = vScale;
}

D3DXMATRIX* Engine::CCollision::GetWorldMatrix(void)
{
	return &m_pInfo->m_matWorld;
}

Engine::OBB* Engine::CCollision::GetOBB(void)
{
	return &m_OBB;
}

D3DXVECTOR3 Engine::CCollision::GetPos(void)
{
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVec3TransformCoord(&vPos, &vPos, &m_pInfo->m_matWorld);

	return vPos;
}

float Engine::CCollision::GetRadius(void)
{
	return m_fRadius;
}

void Engine::CCollision::SetBOOL(bool _Obb, bool _Sphere)
{
	m_bOBB = _Obb;
	m_bSphere = _Sphere;
}

void Engine::CCollision::SetCombineMatrix(const D3DXMATRIX* pMatCombine)
{
	m_pmatCombined = pMatCombine;
}