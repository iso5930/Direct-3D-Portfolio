/*!
 * \file IceBlow.h
 * \date 2016/08/13 16:51
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

#ifndef IceBlow_h__
#define IceBlow_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CShader;
	class CCollision;
}

class CIceBlow : public Engine::CSkill
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CShader*		m_pShader;
	Engine::CDynamicMesh*	m_pMesh;

private:
	list<Engine::CCollision*>	m_pCollisionList;
	Engine::CCollision*			m_pCollision;
	D3DXMATRIX					m_matCombine;
	bool						m_bCollisionCheck;
	bool						m_bCollisionCheck2;
	list<Engine::CCollision*>	m_pCollisionList2;

private:
	UINT	m_iAniIndex;
	bool	m_bCreate;

public:
	static CIceBlow* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int	Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	HRESULT	SetConstantTable(void);

public:
	void	SetInfo(D3DXVECTOR3 vPos);
	void	SetAngel(eAngle _Angel, float fAngle);

public:
	void	Release(void);
	bool	GetCreate(void);
	void	SetCollisionCheck(bool _Check);
	bool	GetCollisionCheck(void);
	void	SetCollisionCheck2(bool _Check);
	bool	GetCollisionCheck2(void);

public:
	list<Engine::CCollision*>*	GetCollisionList(void);
	list<Engine::CCollision*>*	GetCollisionList2(void);
	Engine::CCollision*			GetCollision(void);
	
private:
	explicit CIceBlow(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CIceBlow(void);
};

#endif // IceBlow_h__