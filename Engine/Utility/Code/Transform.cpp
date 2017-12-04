#include "Transform.h"

Engine::CTransform::CTransform(const D3DXVECTOR3& vLook)
:m_vScale(1.f, 1.f, 1.f), m_vPos(0.f, 0.f, 0.f), m_vDir(vLook)
{
	ZeroMemory(m_fAngle, sizeof(float) * Angle_End);
	D3DXMatrixIdentity(&m_matWorld);
}

Engine::CTransform::~CTransform(void)
{
	//Release();
}

Engine::CTransform* Engine::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}

int Engine::CTransform::Update(void)
{
	D3DXMATRIX	matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotX, m_fAngle[Angle_X]);
	D3DXMatrixRotationY(&matRotY, m_fAngle[Angle_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle[Angle_Z]);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;

	return 0; 
}

DWORD Engine::CTransform::Release(void)
{
	return 0;
}