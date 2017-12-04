/*!
 * \file DarkHurricane.h
 * \date 2016/08/25 16:44
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

#ifndef DarkHurricane_h__
#define DarkHurricane_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
	class CCollision;
	class CShader;
}

class CDarkHurricane : public Engine::CSkill
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;

	Engine::CTransform*		m_pInfo2;
	Engine::CStaticMesh*	m_pMesh2;
	Engine::CCollision*		m_pCollision;
	Engine::CShader*		m_pShader;

private:
	D3DXMATRIX				m_matCombine;

	float					m_fAccTime;
	float					m_fMaxTime;

public:
	static CDarkHurricane* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual void Render(void);
	virtual int Update(void);

private:
	HRESULT AddComponent(void);
	HRESULT	SetConstantTable(void);
	HRESULT	SetConstantTable2(void);

public:
	void	SetPos(D3DXVECTOR3 vPos);
	void	SetAngle(float fAngle);
	void	SetDir(D3DXVECTOR3 vDir);

public:
	Engine::CCollision* GetCollision(void);

private:
	explicit CDarkHurricane(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDarkHurricane(void);
};

#endif // DarkHurricane_h__