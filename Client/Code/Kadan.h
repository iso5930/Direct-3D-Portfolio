/*!
 * \file Kadan.h
 * \date 2016/08/11 12:27
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

#ifndef Kadan_h__
#define Kadan_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CStaticMesh;
	class CShader;
	class CCollision;
}

class CPlayer;
class CKadanSpin;

class CKadan : public Engine::CGameObject
{
private:
	Engine::CStaticMesh*	m_pMapMesh;
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
	Engine::CShader*		m_pShader;

private:
	eKadanAniIndex			m_eAniIndex;
	LPD3DXMESH				m_pSphere;
	LPD3DXMESH				m_pSphere2;
	float					m_fRadius;

private:
	CKadanSpin*				m_pSpin[2];
	bool					m_bRage;

private:
	vector<Engine::CCollision*>		m_vecCollision;
	Engine::CCollision*				m_pWeaponCollision;
	CPlayer*						m_pPlayer;
	eKadanState						m_eState;
	float							m_fIdleAccTime;

private:
	bool							m_bMoveCheck;
	float							m_fAttAccTime;
	bool							m_bAttEnd;
	float							m_fBlinkTime;
	bool							m_bBlink;
	bool							m_bCollision;
	float							m_fMoveAccTime;
	bool							m_bCreate;
	float							m_fRunAccTime;

private:
	bool							m_bBlinkEffectCreate;
	bool							m_bFourAttackCheck;

private:
	bool							m_bIce;
	float							m_fIceAccTime;
	
public:
	static CKadan* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(void);

private:
	void	Release(void);
	bool	DirCheck(D3DXVECTOR3 vDir, float fTime, float fSpeed, float& fY);
	void	SetAngle(void);

public:
	void	KadanHit(int iIndex, D3DXVECTOR3 vPlayerPos, bool bSkill = false, eSkillType _Type = Skill_End);
	void	KadanAttack(eKadanAniIndex _Index);
	void	KadanFireShock(void);
	void	InitData(void);
	void	CCollisionUpdate(void);
	bool	CollisionSphereCheck(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir, float _fTime, float _fSpeed);

private:
	void	Collision_Sync(void);

private:
	explicit CKadan(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CKadan(void);
};

#endif // Kadan_h__