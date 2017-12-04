/*!
 * \file Sphere.h
 * \date 2016/07/28 15:30
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

#ifndef Sphere_h__
#define Sphere_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
};

class CSphere : public Engine::CGameObject
{
public:
	Engine::CTransform*	m_pInfo;
	LPD3DXMESH			m_pMesh;
	
public:
	virtual HRESULT Initialize(D3DXVECTOR3 vPos);
	virtual void	Render(void);
	virtual int		Update(void);
	virtual void	Release(void);

public:
	explicit CSphere(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSphere(void);
};

#endif // Sphere_h__