/*!
 * \file IceBomb.h
 * \date 2016/08/25 14:10
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

#ifndef IceBomb_h__
#define IceBomb_h__

#include "Effect.h"

class CIceBomb : public CEffect
{
private:
	float	m_fSpeed;
	float	m_fCurFrame;
	float	m_fEndFrame;

public:
	static CIceBomb* Create(LPDIRECT3DDEVICE9 pDevice);

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
	explicit CIceBomb(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CIceBomb(void);
};

#endif // IceBomb_h__