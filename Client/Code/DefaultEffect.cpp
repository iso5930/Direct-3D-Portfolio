#include "stdafx.h"
#include "Include.h"
#include "DefaultEffect.h"
#include "Export_Function.h"
#include "Transform.h"
#include "RcTex.h"
#include "Texture.h"

CDefaultEffect::CDefaultEffect(LPDIRECT3DDEVICE9 pDevice)
:CEffect(pDevice), m_fAccTime(0.f), m_fAngle(0.f), m_fAngleSpeed(0.f), m_iTextureIndex(0)
, m_bAxis(false), m_fTime(0.f), m_fDirSpeed(0.f) 
{
	ZeroMemory(m_szTextureName, sizeof(TCHAR) * 128);
}

CDefaultEffect::~CDefaultEffect(void)
{

}

CDefaultEffect* CDefaultEffect::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 vPos, D3DXVECTOR3 vStartScale, D3DXVECTOR3 vDir, float fTime, float fDirSpeed, float fAngleSpeed, float fStartAngle, float fAngle, eAngle _Angle, bool bAxis, D3DXVECTOR3 vScale, float fScaleSpeed, PTCHAR pName, int iTexIndex /*= 0*/)
{
	CDefaultEffect* pEffect = new CDefaultEffect(pDevice);

	pEffect->SetTex(iTexIndex, pName);

	if(FAILED(pEffect->Initialize()))
	{
		Engine::Safe_Delete(pEffect);
	}

	pEffect->InitData(vPos, vStartScale, fTime, vDir);
	pEffect->InitData2(vScale, fDirSpeed, fAngleSpeed, fStartAngle, _Angle, bAxis, fScaleSpeed);
	pEffect->SetAngel(fAngle);

	return pEffect;
}

HRESULT CDefaultEffect::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	D3DXMATRIX matBill;
	D3DXMatrixIdentity(&matBill);
	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	D3DXMatrixIdentity(&m_matBill);

	m_matBill._11 = matBill._11;
	m_matBill._13 = matBill._13;
	m_matBill._31 = matBill._31;
	m_matBill._33 = matBill._33;
	D3DXMatrixInverse(&m_matBill, NULL, &m_matBill);

	return S_OK;
}

int CDefaultEffect::Update(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fAccTime += fTime * 1.f;

	if(m_fAccTime >= m_fTime)
	{
		return 1;
	}

	m_pInfo->m_vPos += m_pInfo->m_vDir * m_fDirSpeed * fTime; //ÀÌµ¿	
	m_pInfo->m_vScale += m_vScale * fTime * m_fScaleSpeed;

	if(m_bAxis)
	{
		m_pInfo->m_fAngle[Angle_Y] += m_fAngle * m_fAngleSpeed * fTime;
	}
	else if(m_AngleType != Angle_End)
	{
		m_pInfo->m_fAngle[m_AngleType] += m_fAngle * m_fAngleSpeed * fTime;
	}

	D3DXMATRIX	matScale, matTrans, matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixScaling(&matScale, m_pInfo->m_vScale.x, m_pInfo->m_vScale.y, m_pInfo->m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	if(m_bAxis)
	{
		D3DXMatrixRotationAxis(&matRot, &m_pInfo->m_vDir, m_pInfo->m_fAngle[Angle_Y]);
	}
	else if(m_AngleType != Angle_End)
	{
		switch(m_AngleType)
		{
		case Angle_X:
			D3DXMatrixRotationX(&matRot, m_pInfo->m_fAngle[m_AngleType]);
			break;

		case Angle_Y:
			D3DXMatrixRotationY(&matRot, m_pInfo->m_fAngle[m_AngleType]);
			break;

		case Angle_Z:
			D3DXMatrixRotationZ(&matRot, m_pInfo->m_fAngle[m_AngleType]);
			break;
		}
	}

	m_pInfo->m_matWorld = matScale * matRot * matTrans;
	m_pInfo->m_matWorld = m_matBill * m_pInfo->m_matWorld;

	return 0;
}

void CDefaultEffect::Render(void)
{
	m_pTexture->Render(0, m_iTextureIndex);
	m_pBuffer->Render(&m_pInfo->m_matWorld);
}

HRESULT CDefaultEffect::InitData(D3DXVECTOR3 vPos, D3DXVECTOR3 vScale, float fTime, D3DXVECTOR3 vDir)
{
	m_pInfo->m_vPos = vPos;
	m_pInfo->m_vScale = vScale;
	m_pInfo->m_vDir = vDir;

	m_fTime = fTime;

	return S_OK;
}

HRESULT CDefaultEffect::InitData2(D3DXVECTOR3 vScale, float fDirSpeed, float fAngelSpeed, float fStartAngle, eAngle _Angle, bool bAxis, float fScaleSpeed)
{
	m_vScale = vScale;
	m_fDirSpeed = fDirSpeed;
	m_fAngleSpeed = fAngelSpeed;
	m_AngleType = _Angle;
	m_bAxis = bAxis;
	m_fScaleSpeed = fScaleSpeed;

	if(m_bAxis)
	{
		m_pInfo->m_fAngle[Angle_Y] = fStartAngle;
	}
	else if(_Angle != Angle_End)
	{
		m_pInfo->m_fAngle[_Angle] = fStartAngle;
	}

	return S_OK;
}

HRESULT CDefaultEffect::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, m_szTextureName);
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	return S_OK;
}

void CDefaultEffect::SetSpeed(float _AngleSpeed, float _Angle, float _Speed)
{
	m_fAngleSpeed = _AngleSpeed;
	m_fAngle = _Angle;
	m_fDirSpeed = _Speed;
}

void CDefaultEffect::SetTex(int iIndex, PTCHAR pName)
{
	m_iTextureIndex = iIndex;
	lstrcpy(m_szTextureName, pName);
}

void CDefaultEffect::SetAngel(float fAngle)
{
	m_fAngle = fAngle;
}