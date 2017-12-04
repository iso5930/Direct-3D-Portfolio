/*!
 * \file Camera.h
 * \date 2016/07/08 13:38
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	D3DXMATRIX	m_matView, m_matProj;
	D3DXVECTOR3 m_vEye, m_vAt, m_vUp;
	float		m_fFovY, m_fAspect, m_fNear, m_fFar;

public:
	virtual int Update(void);

public:
	void Invalidate_ViewMatrix(void);
	void Invalidate_ProjMatrix(void);

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCamera(void);
};

END

#endif // Camera_h__