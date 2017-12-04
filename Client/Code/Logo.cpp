#include "stdafx.h"
#include "Logo.h"
#include "Export_Function.h"
#include "LogoBack.h"
#include "Layer.h"
#include "LogoBack.h"
#include "Loading.h"
#include "SceneSelector.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
:CScene(pDevice), m_pResourceMgr(Engine::Get_ResourceMgr()), m_pManagement(Engine::Get_Management()), m_pLoading(NULL)
{

}

CLogo::~CLogo(void)
{
	Release();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pLogo = new CLogo(pDevice);

	if(FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CLogo::Initialize(void)
{
	HRESULT hr = NULL;

	hr = m_pResourceMgr->AddTexture(m_pDevice, Resource_Logo, Texture_Normal, L"Texture_Logo", L"../bin/Resource/Logo/Hero.jpg", 1);

	m_pLoading = CLoading::Create(Load_Stage);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");

	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();
	
	if(m_pLoading->GetComplete() == true)
	{
		if(Engine::Get_InputMgr()->GetDIKeyState(DIK_RETURN) & 0x80)
		{
			HRESULT hr = m_pManagement->SceneChange(CSceneSelector(SceneID_Stage));
			FAILED_CHECK_RETURN_MSG(hr, ,L"Scene Change Failed");

			return;
		}
	}
}

void CLogo::Render(void)
{
	//Engine::CScene::Render();

	Engine::Get_FontMgr()->Render(L"궁서", D3DXVECTOR3(100.f, 500.f, 0.f), m_pLoading->GetLoadingMessage(), D3DCOLOR_ARGB(255, 0, 255, 0));
}

HRESULT CLogo::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CLogoBack::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pLayer, E_FAIL, L"LogoBack 생성 실패");

	pLayer->AddObject(L"LogoBack", pGameObject);

	Engine::Get_Management()->Add_RenderGroup(Type_Priority, pGameObject);

	m_mapLayer.insert(map<WORD, Engine::CLayer*>::value_type(Layer_GameLogic, pLayer));

	return S_OK;
}

HRESULT CLogo::Add_Light(void)
{
	D3DLIGHT9 LightInfo;

	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(0.f, 0.f, 1.f);

	HRESULT hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	return S_OK;
}

void CLogo::Release(void)
{
	Engine::Get_ResourceMgr()->ResourceReset(Resource_Logo);
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Safe_Delete(m_pLoading);
}