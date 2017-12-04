/*!
 * \file DarkBomb2.h
 * \date 2016/08/26 7:52
 *
 * \author Jin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef DarkBomb2_h__
#define DarkBomb2_h__

#include "Effect.h"

class CDarkBomb2 : public CEffect
{
private:
	float	m_fSpeed;
	float	m_fCurFrame;
	float	m_fEndFrame;

public:
	static CDarkBomb2* Create(LPDIRECT3DDEVICE9 pDevice);

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
	explicit CDarkBomb2(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDarkBomb2(void);
};


#endif // DarkBomb2_h__