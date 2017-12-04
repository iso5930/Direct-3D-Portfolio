/*!
 * \file FireBolt.h
 * \date 2016/08/16 13:24
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

#ifndef FireBolt_h__
#define FireBolt_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CCollision;
}

class CFireBolt : public Engine::CSkill
{
private:
	Engine::CTransform* m_pInfo;
	Engine::CVIBuffer*	m_pBuffer;
	Engine::CTexture*	m_pTexture;
	Engine::CCollision*	m_pCollision;
	D3DXMATRIX			m_matCombine;

private:
	float	m_fTime;
	float	m_fAccTime;
	float	m_fCurFrame;
	float	m_fEndFrame;
	float	m_fRadius;

public:
	static	CFireBolt* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);
	
public:
	virtual void Render(void);
	virtual int Update(void);

private:
	HRESULT AddComponent(void);
	void	Release(void);

public:
	Engine::CTransform* GetInfo(void);
	Engine::CCollision*	GetCollision(void);

private:
	explicit CFireBolt(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFireBolt(void);
};

#endif // FireBolt_h__