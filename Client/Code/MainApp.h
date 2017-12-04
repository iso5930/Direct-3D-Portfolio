/*!
 * \file MainApp.h
 * \date 2016/06/29 11:20
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

#ifndef MainApp_h__
#define MainApp_h__

#include "Include.h"

namespace Engine
{
	class CGraphicDevice;
	class CManagement;
	class CResourcsMgr;
	class CTimeMgr;
}

class CMainApp
{
private:	
	LPDIRECT3DDEVICE9		m_pDevice;
	Engine::CGraphicDevice*	m_pGraphicDev;
	Engine::CManagement*	m_pManagement;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CResourcsMgr*	m_pResourceMgr;

public:
	static CMainApp* Create(void);

private:
	HRESULT Init_MainApp(void);
	void	Release(void);

public:
	void Update(void);
	void Render(void);

private:
	CMainApp(void);

public:
	~CMainApp(void);
};

#endif // MainApp_h__