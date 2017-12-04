/*!
 * \file Player.h
 * \date 2016/07/25 18:44
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

#ifndef Player_h__
#define Player_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CStaticMesh;
	class CShader;
	class CCollision;
	class CTexture;
}

class CKadan;
class CManaAmber;

class CPlayer : public Engine::CGameObject
{
private:
	Engine::CStaticMesh*	m_pMapMesh;
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
	Engine::CShader*		m_pShader;
	CManaAmber*				m_pManaAmber;
	Engine::CTexture*		m_pSwordTexture;

private:
	DWORD	m_dwIndex;
	UINT	m_iAniIndex;
	bool	m_bPush;
	bool	m_bDarkKnightTrans;

private:
	ePlayerState	m_eState;
	ePlayerState	m_ePrevState;
	float			m_fCharge;
	float			m_fWaitTime;
	bool			m_bCharge;
	bool			m_bMagicCreate;
	bool			m_bChargeComplete;

private:
	bool			m_bTestPush;
	bool			m_bStrongPush;
	
private:
	eDarkKnightType	m_eDarkKnightType;
	bool			m_bAttckFlag;
	DWORD			m_dwKey;
	bool			m_bBlink;
	float			m_fBlinkAccTime;
	bool			m_bBlinkStart;
	bool			m_bBlinkEffect;

private:
	vector<Engine::CCollision*> m_vecCollision;
	vector<Engine::CCollision*> m_vecWeaponCollision;
	LPD3DXMESH					m_pShere;
	float						m_fRadius;
	float						m_fChargeAccTime;
	float						m_fChargeTime; //스킬마다 다름.
	float						m_fReturnNum;

private:
	bool						m_bCollisionPush[3];

private:
	deque<Engine::SWORDEFFECT*> m_dequeScythEffect;
	float						m_fAccEffectTime;	
	const	D3DXMATRIX*			m_pMatWeapon;
	const	D3DXMATRIX*			m_pMatWeapon2;
	const	D3DXMATRIX*			m_pMatWeapon3;
	float						m_fDeleteTime;
	bool						m_bScytheEffectCheck;

private:
	LPD3DXMESH					m_SlideCollision;
	float						m_fSlideRadius;
	bool						m_bTransformPush;

private:
	CKadan*						m_pMonster;
		
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(void);
	bool	KeyCheck(void);
	bool	ScytheKeyCheck(void);
	bool	SetHeight(D3DXVECTOR3 _vDir, float fTime, float fSpeed);
	bool	WallCheck(void);
	void	Release(void);

private:
	int		DarkKnight_Staff_Update(void);
	int		DarkKnight_Scythe_Update(void);

public:
	void	SetCustomColor(D3DXVECTOR4 vCustomColor, eCustomParts _Parts, eCustomColor _Color);
	void	SetDir(DWORD _dwKey);

private:
	void	SetState(ePlayerAniIndex _AniIndex);
	void	Collision_Sync(void);

public:
	ePlayerState	GetState(void);
	Engine::CDynamicMesh*	GetMesh(void);
	bool			GetChargeComplate(void);
	UINT			GetAniIndex(void);
	bool			CollisionCheck(void);
	void			SetScytheState(eDarkKnightAniIndex _AniIndex);
	Engine::CTransform*	GetInfo(void);
	void			PlayerHit(int iIndex, D3DXVECTOR3 vPos, bool _bStrong);
	bool			CollisionSphereCheck(D3DXVECTOR3 _vPos, D3DXVECTOR3 vDir, float fTime, float fSpeed);
	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayer(void);
};

#endif // Player_h__