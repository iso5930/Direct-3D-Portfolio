/*!
 * \file DarkBomb.h
 * \date 2016/08/26 2:48
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

#ifndef DarkBomb_h__
#define DarkBomb_h__

#include "Effect.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
}

class CDarkBomb : public CEffect
{
private:
	Engine::CStaticMesh* m_pMesh;
	Engine::CShader*	m_pShader;

private:
	float				m_fAccTime;
	float				m_fMaxTime;

public:
	static CDarkBomb* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int	Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	HRESULT	SetConstantTable(void);

public:
	void	SetPos(D3DXVECTOR3 vPos);

private:
	explicit CDarkBomb(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDarkBomb(void);
};

#endif // DarkBomb_h__