#include "stdafx.h"
#include "Stage.h"
#include "Layer.h"
#include "DynamicCamera.h"
#include "Map.h"
#include "Player.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Magic_UI.h"
#include "Kadan.h"
#include "EffectMgr.h"
#include "CustomColorUI.h"
#include "IceBlow.h"
#include "SkyBox.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
:Engine::CScene(pDevice)
{

}

CStage::~CStage(void)
{
	Release();
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pStage = new CStage(pDevice);

	if(FAILED(pStage->Initialize()))
	{
		Engine::Safe_Delete(pStage);
	}

	return pStage;
}

HRESULT CStage::Initialize(void)
{
	HRESULT hr = NULL;
	
	Engine::Get_Management()->RenderGroupRelease();

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Add_Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"Add_GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");

	Engine::Get_ShadowMgr()->InitShadowMgr();

	CSoundMgr::GetInstance()->PlayBGM(L"KadanBgm.mp3");

	Engine::Get_Management()->m_pRenderer->Init_Blur();

	return S_OK;
}

void CStage::Update(void)
{
	Engine::CScene::Update();

	CCollisionMgr::GetInstance()->SkillToMonster();
	CCollisionMgr::GetInstance()->SkillToPlayer();

	CEffectMgr::GetInstance()->Update();
}

void CStage::Render(void)
{
	//Engine::CScene::Render();
	
	//Engine::Get_NaviMgr()->Render();

	CEffectMgr::GetInstance()->Render();
}

HRESULT CStage::Add_Environment_Layer(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CMap::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Map", pGameObject);
	Engine::Get_Management()->Add_RenderGroup(Type_Shader, pGameObject);

	m_mapLayer.insert(map<WORD, Engine::CLayer*>::value_type(Layer_Environment, pLayer));

	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert(map<WORD, Engine::CLayer*>::value_type(Layer_GameLogic, pLayer));

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Player", pGameObject);
	Engine::Get_Management()->Add_RenderGroup(Type_Shader, pGameObject);	

	Engine::CComponent* pComponent = this->GetComponent(Layer_GameLogic, L"Player", L"Transform");

	D3DXVECTOR3* pPos = &((Engine::CTransform*)pComponent)->m_vPos;

	pGameObject = CDynamicCamera::Create(m_pDevice, &dynamic_cast<Engine::CTransform*>(pComponent)->m_vPos);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Camera", pGameObject);
	
	pGameObject = CKadan::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Kadan", pGameObject);
	Engine::Get_Management()->Add_RenderGroup(Type_Shader, pGameObject);

	return S_OK;
}

HRESULT CStage::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(0.f, -1.f, 1.f);

	HRESULT hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	return S_OK;
}

void CStage::Release(void)
{

}

HRESULT CStage::Add_UI_Layer(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CSkyBox::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"SkyBox", pGameObject);
	Engine::Get_Management()->Add_RenderGroup(Type_Priority, pGameObject);	

	pGameObject = CMagic_UI::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Magic_UI", pGameObject);
	Engine::Get_Management()->Add_RenderGroup(Type_UI, pGameObject);

	pGameObject = CCustomColor_UI::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"CustomColor_UI", pGameObject);
	Engine::Get_Management()->Add_RenderGroup(Type_UI, pGameObject);

	m_mapLayer.insert(map<WORD, Engine::CLayer*>::value_type(Layer_UI, pLayer)); 

	return S_OK;
}