/*!
 * \file KadanSpin.h
 * \date 2016/08/25 18:28
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

#ifndef KadanSpin_h__
#define KadanSpin_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CShader;
	class CStaticMesh; 
}

class CKadanSpin : public Engine::CGameObject
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CShader*		m_pShader;
	Engine::CStaticMesh*	m_pMesh;

public:
	static CKadanSpin* Create(LPDIRECT3DDEVICE9 pDevice);

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
	void	SetAngle(float fAngle);

private:
	explicit CKadanSpin(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CKadanSpin(void);
};

#endif // KadanSpin_h__