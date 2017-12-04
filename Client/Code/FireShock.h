/*!
 * \file FireShock.h
 * \date 2016/08/15 23:04
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

#ifndef FireShock_h__
#define FireShock_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CFireShock : public Engine::CSkill
{
private:
	Engine::CTransform* m_pInfo;
	Engine::CVIBuffer*	m_pBuffer;
	Engine::CTexture*	m_pTexture;
	
private:
	float				m_fAccTime;
	float				m_fTime;
	float				m_fRadius;
	
public:
	static CFireShock* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);
	
public:
	virtual void Render(void);
	virtual int Update(void);

private:
	HRESULT AddComponent(void);

public:
	Engine::CTransform* GetInfo(void);
	
private:
	explicit CFireShock(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFireShock(void);
};

#endif // FireShock_h__