/*!
 * \file BlinkEffect.h
 * \date 2016/08/22 21:48
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

#ifndef BlinkEffect_h__
#define BlinkEffect_h__

#include "Effect.h"

class CBlinkEffct : public CEffect
{
private:
	float	m_fSpeed;
	float	m_fCurFrame;
	float	m_fEndFrame;
		
public:
	static CBlinkEffct* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int		Update(void);
	virtual void	Render(void);

private:
	void	Release(void);

public:
	void	SetPos(D3DXVECTOR3 vPos);
	
private:
	CBlinkEffct(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBlinkEffct(void);
};

#endif // BlinkEffect_h__