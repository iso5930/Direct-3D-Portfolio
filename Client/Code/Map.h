/*!
 * \file Map.h
 * \date 2016/07/19 13:50
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

#ifndef Map_h__
#define Map_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CResourcsMgr;
	class CStaticMesh;
	class CShader;
}

class CMap : public Engine::CGameObject
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CResourcsMgr*	m_pResourceMgr;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CShader*		m_pShader;

public:
	static CMap* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);
	HRESULT	SetConstantTable(void);
	void	Release(void);

public:
	Engine::CStaticMesh* GetMesh(void);
	
private:
	CMap(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMap(void);
};

#endif // Map_h__