#include "stdafx.h"
#include "Include.h"
#include "DynamicCamera.h"
#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
:Engine::CCamera(pDevice), m_fCamSpeed(0.f), m_pTarget(NULL), m_bMouse(true), m_bPush(false), m_bDarkKnight(false)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pTarget)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pDevice);
	
	pCamera->SetTarget(pTarget);

	if(FAILED(pCamera->Initialize(pTarget)))
	{
		Engine::Safe_Delete(pCamera);
	}
	
	return pCamera;
}

int CDynamicCamera::Update(void)
{
	KeyCheck();

 	if(m_bMouse)
 	{
		FixMouse();
		MouseMove();
 	}

	if(m_bDarkKnight)
	{
		D3DXVECTOR3 vPos(0.f, 0.f, 0.f);
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		matWorld = (*m_pmatCombined) * (*m_pmatInfo);
		//D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);

		D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);
		m_vEye = vPos;

		m_vAt = (*m_pTarget) + D3DXVECTOR3(0.f, 50.f, 0.f);
	}
	else
	{
		m_vAt = (*m_pTarget) + D3DXVECTOR3(0.f, 50.f, 0.f);
		D3DXVECTOR3 vcZ = D3DXVECTOR3(m_matView._13, m_matView._23, m_matView._33);
		m_vEye = m_vAt - vcZ * m_fDist;
	}
	
	Engine::CCamera::Update();

	return 0;
}

void CDynamicCamera::KeyCheck(void)
{
// 	float fTime = Engine::Get_TimeMgr()->GetTime();
// 
// 	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_A) & 0x80)
// 	{
// 		D3DXVECTOR3		vRight;
// 		D3DXMATRIX		matCamState;
// 
// 		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
// 		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
// 		D3DXVec3Normalize(&vRight, &vRight);
// 
// 		m_vEye -= vRight * m_fCamSpeed * fTime;
// 		m_vAt -= vRight * m_fCamSpeed * fTime;
// 	}
// 
// 	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_D) & 0x80)
// 	{
// 		D3DXVECTOR3		vRight;
// 		D3DXMATRIX		matCamState;
// 
// 		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
// 		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
// 		D3DXVec3Normalize(&vRight, &vRight);
// 
// 		m_vEye += vRight * m_fCamSpeed * fTime;
// 		m_vAt += vRight * m_fCamSpeed * fTime;
// 	}
// 
// 	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_W) & 0x80)
// 	{
// 		D3DXVECTOR3		vLook;
// 		vLook = m_vAt - m_vEye;
// 		D3DXVec3Normalize(&vLook, &vLook);
// 
// 		m_vEye += vLook * m_fCamSpeed * fTime;
// 		m_vAt += vLook * m_fCamSpeed * fTime;
// 	}
// 
// 	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_S) & 0x80)
// 	{
// 		D3DXVECTOR3		vLook;
// 		vLook = m_vAt - m_vEye;
// 		D3DXVec3Normalize(&vLook, &vLook);
// 
// 		m_vEye -= vLook * m_fCamSpeed * fTime;
// 		m_vAt -= vLook * m_fCamSpeed * fTime;
// 	}
// 
 	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_1) & 0x80)
 	{
		if(m_bPush == true)
		{
			return;
		}

		m_bPush = true;

		if(m_bMouse)
		{
			m_bMouse = false;
		}
		else
			m_bMouse = true;
 	}
	else
		m_bPush = false;
}

HRESULT CDynamicCamera::Initialize(D3DXVECTOR3* pTarget)
{
	D3DXVECTOR3 vcZ = D3DXVECTOR3(m_matView._13, m_matView._23, m_matView._33);

	m_fDist = 150.f;

	m_vAt = (*m_pTarget);

	m_vAt.y += 50.f;

	m_vEye = m_vAt - vcZ * m_fDist;

	m_fCamSpeed = 150.f;
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINCX) / float(WINCY);
	m_fNear = 1.f;
	m_fFar = 10000.f;
	Invalidate_ProjMatrix();

	return S_OK;
}

void CDynamicCamera::FixMouse(void)
{
	POINT		ptMouse = {WINCX >> 1, WINCY >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::MouseMove(void)
{
	if(m_bDarkKnight)
	{
		return;
	}

	int		iDistance = 0;

	if(iDistance = Engine::Get_InputMgr()->GetDIMouseMove(DIM_X))
	{
		//Y축 회전
		float fAngle;

		D3DXVECTOR3 vZ;
		D3DXVECTOR3 vY;

		D3DXMATRIX matRotY;
		
		fAngle = D3DXToRadian(iDistance * 0.1f);
		D3DXMatrixRotationY(&matRotY, fAngle);

		vZ = m_vEye - m_vAt;
		vY = D3DXVECTOR3(m_matView._12, m_matView._22, m_matView._32);

		D3DXVec3TransformCoord(&vZ, &vZ, &matRotY);
		D3DXVec3TransformCoord(&vY, &vY, &matRotY);

		m_vEye = m_vAt + vZ;
		m_vUp = vY;
		
		CCamera::Invalidate_ViewMatrix();
	}

	if(iDistance = Engine::Get_InputMgr()->GetDIMouseMove(DIM_Y))
	{
		//X축 회전
		float fAngle;
		D3DXMATRIX matRotX;
		D3DXVECTOR3 vX, vY, vZ;

		fAngle = D3DXToRadian(iDistance * 0.1f);

		vZ = m_vEye - m_vAt;
		vY = D3DXVECTOR3(m_matView._12, m_matView._22, m_matView._32);
		vX = D3DXVECTOR3(m_matView._11, m_matView._21, m_matView._31);

		D3DXMatrixRotationAxis(&matRotX, &vX, fAngle);
		D3DXVec3TransformCoord(&vZ, &vZ, &matRotX);
		D3DXVec3TransformCoord(&vY, &vY, &matRotX);

 		/*D3DXVECTOR3 vPos = m_vEye;
 		vPos.y = m_vAt.y;
 
 		D3DXVECTOR3 vAt = m_vEye - m_vAt;
 		D3DXVECTOR3 vLook = vPos - m_vAt;
 
 		D3DXVec3Normalize(&vAt, &vAt);
 		D3DXVec3Normalize(&vLook, &vLook);
 
 		float fResult = D3DXVec3Dot(&vLook, &vAt);
 		fResult = acosf(fResult);
 
 		fResult = D3DXToDegree(fResult);
 
 		if(fResult >= 15.f && fResult <= 27.f)
 		{
		
		}*/

		m_vEye = m_vAt + vZ;
		m_vUp = vY;

		CCamera::Invalidate_ViewMatrix();
	}

	if(iDistance = Engine::Get_InputMgr()->GetDIMouseMove(DIM_Z))
	{
		m_fDist -= iDistance * Engine::Get_TimeMgr()->GetTime();

		if(m_fDist >= 300.f)
		{
			m_fDist = 300.f;
		}
		else if(m_fDist <= 50.f)
		{
			m_fDist = 50.f;
		}
	}
}

void CDynamicCamera::SetTarget(D3DXVECTOR3* pPos)
{
	m_pTarget = pPos;
}

D3DXVECTOR3 CDynamicCamera::GetRight(void)
{
	D3DXVECTOR3 vPos = D3DXVECTOR3(m_matView._11, m_matView._21, m_matView._31);

	D3DXVec3Normalize(&vPos, &vPos);

	return vPos;
}

void CDynamicCamera::SetDarkKnight(bool _bTrans)
{
	m_bDarkKnight = _bTrans;


	if(_bTrans == false)
	{
 		m_matView = m_matPrevView;
 		m_vEye = m_vPrevEye;
		
		Invalidate_ViewMatrix();
		Invalidate_ProjMatrix();
	}
	else
	{
 		m_vPrevEye = m_vEye;
 		m_matPrevView = m_matView;
	}
}

void CDynamicCamera::SetMatrix(const D3DXMATRIX* pMatInfo, const D3DXMATRIX* pmatCombined)
{
	m_pmatInfo = pMatInfo;
	m_pmatCombined = pmatCombined;
}