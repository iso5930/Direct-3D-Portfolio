#include "stdafx.h"
#include "Camera_Tool.h"
#include "Export_Function.h"

CCamera_Tool::CCamera_Tool(LPDIRECT3DDEVICE9 pDevice)
:CCamera(pDevice), m_fSpeed(0.f)
{

}

CCamera_Tool::~CCamera_Tool(void)
{

}

HRESULT CCamera_Tool::Initialize(D3DXVECTOR3 vEye, D3DXVECTOR3 vAt)
{
	m_fSpeed = 150.f;
	m_vEye = vEye;
	m_vAt = vAt;
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINCX) / float(WINCY);
	m_fNear = 1.f;
	m_fFar = 3000.f;
	Invalidate_ProjMatrix();

	return S_OK;
}	

void CCamera_Tool::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_A) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fSpeed * fTime;
		m_vAt -= vRight * m_fSpeed * fTime;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_D) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fSpeed * fTime;
		m_vAt += vRight * m_fSpeed * fTime;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_W) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fSpeed * fTime;
		m_vAt += vLook * m_fSpeed * fTime;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_S) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fSpeed * fTime;
		m_vAt -= vLook * m_fSpeed * fTime;
	}

	/*int iDistance; - 12

	if(iDistance = Engine::Get_InputMgr()->GetDIMouseMove(DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}*/

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_LEFT) & 0x80)
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(-6 / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(6 / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_UP) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(-6 / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_DOWN) & 0x80)
	{	
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(6 / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}

int CCamera_Tool::Update(void)
{
	KeyCheck();

	Engine::CCamera::Update();

	return 0;
}