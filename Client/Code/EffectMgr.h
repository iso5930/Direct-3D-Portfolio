/*!
 * \file EffectMgr.h
 * \date 2016/08/11 15:38
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

#ifndef EffectMgr_h__
#define EffectMgr_h__

#include "Include.h"

class CEffect;

class CEffectMgr
{
public:
	DECLARE_SINGLETON(CEffectMgr);

private:
	list<CEffect*>		m_EffectList;
	LPDIRECT3DDEVICE9	m_pDevice;

public:
	void	AddEffect(CEffect* pEffect);
	void	Update(void);
	void	Render(void);

private:
	void	Release(void);

private:
	CEffectMgr(void);
	~CEffectMgr(void);
};

#endif // EffectMgr_h__