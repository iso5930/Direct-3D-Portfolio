/*!
 * \file CCollisionMgr.h
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

#ifndef CCollisionMgr_h__
#define CCollisionMgr_h__

#include "Engine_Include.h"

namespace Engine
{
	class CCollision;
}

class CIceBlow;

class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr);

private:
	vector<Engine::CCollision*>*	m_pvecCollision[2]; //플레이어와 몬스터
	Engine::OBB						m_OBB[2];
	D3DXVECTOR3*					m_pPlayerPos;
	D3DXVECTOR3*					m_pKadanPos;
	float							m_fPlayerRadius;
	float							m_fKadanRadius;

private:
	vector<Engine::CCollision*>*	m_pvecWeaponPlayer;	
	Engine::CCollision*				m_pWeaponKadan;

private:
	void	InitAxis(Engine::OBB* pOBB);

public:
	bool	Collision_OBB(Engine::CCollision* pDestOBB, Engine::CCollision* PSourOBB);
	bool	Collision_Sphere(D3DXVECTOR3 vDestPos, float fDestRadius, D3DXVECTOR3 vSourPos, float fSourRadius);
	bool	Collision_OBB_To_Sphere(Engine::CCollision* pDestOBB, D3DXVECTOR3 vSourPos, float fSourRadius);
	bool	Collision_IceBlow(CIceBlow* pSkill);
	bool	Collision_IceBlow2(CIceBlow* pSkill);

public:
	int		PlayerToKadan(void);
	int		KadanToPlayer(void);
	bool	SkillToMonster(void);
	bool	SkillToPlayer(void);

public:
	void	AddData(int iIndex, vector<Engine::CCollision*>* pvecCollision);
	void	InitData(D3DXVECTOR3* vPos, float fRadius, eCollisionType _Type);

public:
	void	PlayerWeapon(vector<Engine::CCollision*>* pvecPlayerWeapon);
	void	KadanWeapon(Engine::CCollision* pWeaponCollision);

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);
};

#endif // CCollisionMgr_h__