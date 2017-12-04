/*!
 * \file IceDust.h
 * \date 2016/08/16 11:14
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

#ifndef IceDust_h__
#define IceDust_h__

#include "Effect.h"

class CIceDust : public CEffect
{
private:
	float	m_fSpeed;
	float	m_fCurFrame;
	float	m_fEndFrame;

public:
	static CIceDust* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual	int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);

public:
	void	SetInfo(D3DXVECTOR3 vPos);

private:
	explicit CIceDust(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CIceDust(void);
};

#endif // IceDust_h__