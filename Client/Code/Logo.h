/*!
 * \file Logo.h
 * \date 2016/07/06 22:48
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

#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"

namespace Engine
{
	class CResourcsMgr;
	class CManagement;
}

class CLoading;

class CLogo : public Engine::CScene
{
private:
	Engine::CResourcsMgr*	m_pResourceMgr;
	Engine::CManagement*	m_pManagement;
	
private:
	CLoading*				m_pLoading;

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_Light(void);
	void	Release(void);

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogo(void);
};

#endif // Logo_h__