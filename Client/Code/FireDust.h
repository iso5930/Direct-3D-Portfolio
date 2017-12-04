/*!
 * \file FireDust.h
 * \date 2016/08/16 15:11
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

#ifndef FireDust_h__
#define FireDust_h__

#include "Effect.h"

class CFireDust : public CEffect
{
private:
	float	m_fSpeed;
	float	m_fCurFrame;
	float	m_fEndFrame;

public:
	static CFireDust* Create(LPDIRECT3DDEVICE9 pDevice);

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
	explicit CFireDust(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFireDust(void);
};


#endif // FireDust_h__