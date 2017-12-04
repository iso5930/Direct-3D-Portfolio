/*!
 * \file MagicArrow.h
 * \date 2016/08/14 1:04
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

#ifndef MagicArrow_h__
#define MagicArrow_h__

#include "Skill.h"

namespace Engine
{
	class CTransform;
	class CCollision;
	class CVIBuffer;
	class CTexture;
	class CStaticMesh;
	class CShader;
	class CCollision;
}

class CMagicArrowEffect;

class CMagicArrow : public Engine::CSkill
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;
	Engine::CCollision*		m_pCollision;
	D3DXMATRIX				m_matCombine;

private:
	float					m_fTime;
	float					m_fAccTime;

private:
	CMagicArrowEffect*		m_pEffect;

public:
	static	CMagicArrow* Create(LPDIRECT3DDEVICE9 pDevice);

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
	void	SetTime(float fTime);
	void	SetDir(D3DXVECTOR3 vDir);
	void	SetAngle(float fAngle);

public:
	Engine::CCollision* GetCollision(void);

private:
	explicit CMagicArrow(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMagicArrow(void);
};

#endif // MagicArrow_h__