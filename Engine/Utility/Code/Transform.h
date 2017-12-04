/*!
 * \file Transform.h
 * \date 2016/07/07 21:28
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

#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vDir;
	D3DXVECTOR3 m_vScale;
	D3DXMATRIX	m_matWorld;
	float		m_fAngle[Angle_End];

public:
	static CTransform* Create(const D3DXVECTOR3& vLook);

public:
	virtual int		Update(void);
	virtual DWORD	Release(void);

private:
	explicit CTransform(const D3DXVECTOR3& vLook);

public:
	virtual ~CTransform(void);
};

END

#endif // Transform_h__