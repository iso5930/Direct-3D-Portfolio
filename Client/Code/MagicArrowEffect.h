/*!
 * \file MagicArrowEffect.h
 * \date 2016/08/25 19:12
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

#ifndef MagicArrowEffect_h__
#define MagicArrowEffect_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CShader;
	class CStaticMesh; 
}

class CMagicArrowEffect : public Engine::CGameObject
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CShader*		m_pShader;
	Engine::CStaticMesh*	m_pMesh;

private:
	float					m_fAngle;

public:
	static CMagicArrowEffect* Create(LPDIRECT3DDEVICE9 pDevice);

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
	void	SetDir(D3DXVECTOR3 vDir);

private:
	explicit CMagicArrowEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMagicArrowEffect(void);
};


#endif // MagicArrowEffect_h__