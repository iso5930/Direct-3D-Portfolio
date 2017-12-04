/*!
 * \file FireBomb.h
 * \date 2016/08/16 13:38
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

#ifndef FireBomb_h__
#define FireBomb_h__

#include "Effect.h"

class CFireBomb : public CEffect
{
private:
	float	m_fSpeed;
	float	m_fCurFrame;
	float	m_fEndFrame;

public:
	static CFireBomb* Create(LPDIRECT3DDEVICE9 pDevice);

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
	explicit CFireBomb(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFireBomb(void);
};

#endif // FireBomb_h__