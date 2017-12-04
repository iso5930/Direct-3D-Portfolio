#include "stdafx.h"
#include "MainApp.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "CollisionMgr.h"
#include "EffectMgr.h"
#include "SoundMgr.h"

CMainApp::CMainApp(void)
: m_pGraphicDev(Engine::Get_GraphicDev()), m_pManagement(Engine::Get_Management()),
m_pTimeMgr(Engine::Get_TimeMgr()), m_pResourceMgr(Engine::Get_ResourceMgr())
{
// 	AllocConsole(); 
// 	freopen( "CONOUT$",  "wt", stdout);
// 	SetConsoleTitleA("Debug");
}

CMainApp::~CMainApp(void)
{
	//FreeConsole();
	Release();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp* pMainApp = new CMainApp;

	if(FAILED(pMainApp->Init_MainApp()))
	{
		Engine::Safe_Delete(pMainApp);
	}

	return pMainApp;
}

HRESULT CMainApp::Init_MainApp(void)
{
	HRESULT		hr = NULL;

	hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDevice::MODE_WIN, g_hWnd, WINCX, WINCY);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"디바이스 생성 실패");

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	hr = Engine::Get_InputMgr()->InitInputDevice(g_hInst, g_hWnd);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"InputMgr Init Failed");

	hr = Engine::Get_FontMgr()->AddFont(m_pDevice, L"궁서", 30, 18, FW_HEAVY);		FAILED_CHECK(hr);

 	hr = m_pManagement->InitManagement(m_pDevice);
 	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Management Init Failed");

	hr = m_pResourceMgr->ReserveContainerSize(Resource_End);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"리소스 컨테이너 할당 실패");

	hr = m_pResourceMgr->AddBuffer(m_pDevice, Resource_Static, Buffer_RcTex, L"Buffer_RcTex");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Buffer_RcTex 생성 실패");

	hr = m_pManagement->SceneChange(CSceneSelector(SceneID_Logo));

	m_pTimeMgr->InitTime();

	return S_OK;
}

void CMainApp::Release(void)
{
	//Client
	CCollisionMgr::GetInstance()->DestroyInstance();
	CEffectMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();

	//System
	Engine::Safe_Single_Destory(m_pGraphicDev);
	Engine::Safe_Single_Destory(m_pTimeMgr);
	Engine::Get_FontMgr()->DestroyInstance();
	Engine::Get_InputMgr()->DestroyInstance();

	//Utility
	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_ShaderMgr()->DestroyInstance();
	Engine::Get_NaviMgr()->DestroyInstance();
	Engine::Get_SkillMgr()->DestroyInstance();
	Engine::Get_ShadowMgr()->DestroyInstance();

	//Resource
	Engine::Safe_Single_Destory(m_pResourceMgr);
}

void CMainApp::Update(void)
{
	m_pTimeMgr->SetTime();
	Engine::Get_InputMgr()->SetInputState();
	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}