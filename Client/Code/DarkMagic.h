/*!
 * \file DarkMagic.h
 * \date 2016/08/26 3:05
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

#ifndef DarkMagic_h__
#define DarkMagic_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CDarkMagic : public Engine::CSkill
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
	static CDarkMagic* Create(LPDIRECT3DDEVICE9 pDevice);

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
	explicit CDarkMagic(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDarkMagic(void);
};

#endif // DarkMagic_h__