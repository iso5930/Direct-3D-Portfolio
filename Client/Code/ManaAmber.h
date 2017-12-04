/*!
 * \file ManaAmber.h
 * \date 2016/08/16 16:14
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

#ifndef ManaAmber_h__
#define ManaAmber_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
	class CCollision;
	class CShader;
}

class CManaAmber : public Engine::CGameObject
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CCollision*		m_pCollision;
	Engine::CShader*		m_pShader;

private:
	bool					m_bView;
	D3DXMATRIX				m_matCombine;

public:
	static CManaAmber* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual void Render(void);
	virtual int Update(void);

private:
	HRESULT AddComponent(void);
	HRESULT	SetConstantTable(void);

public:
	void	SetPos(D3DXVECTOR3 vPos);
	void	SetView(bool _View);
	
private:
	explicit CManaAmber(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CManaAmber(void);
};

#endif // ManaAmber_h__