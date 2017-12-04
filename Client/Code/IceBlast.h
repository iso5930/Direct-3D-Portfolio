/*!
 * \file IceBlast.h
 * \date 2016/08/16 1:34
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

#ifndef IceBlast_h__
#define IceBlast_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
	class CCollision;
}

class CIceBlast : public Engine::CSkill
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CCollision*		m_pCollision;

private:
	D3DXMATRIX			m_matCombine;
	float				m_fAccTime;
	float				m_fTime;
	float				m_fAngle;

public:
	static CIceBlast* Create(LPDIRECT3DDEVICE9 pDevice);

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
	explicit CIceBlast(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CIceBlast(void);
};

#endif // IceBlast_h__