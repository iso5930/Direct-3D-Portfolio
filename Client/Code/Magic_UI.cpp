#include "stdafx.h"
#include "Include.h"
#include "Magic_UI.h"
#include "Export_Function.h"
#include "2DSprite.h"
#include "Player.h"
#include "DynamicMesh.h"

CMagic_UI::CMagic_UI(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pSprite(NULL), m_fCharge(0.f)
{

}

CMagic_UI::~CMagic_UI(void)
{
	m_pSprite->Release();
}

CMagic_UI* CMagic_UI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMagic_UI*		pGameObject = new CMagic_UI(pDevice);

	if(FAILED(pGameObject->Initialize()))
	{
		Engine::Safe_Delete(pGameObject);
	}

	return pGameObject;
}	

HRESULT CMagic_UI::Initialize(void)
{	
	FAILED_CHECK(AddComponent());

	m_bEnable = false;

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CMagic_UI::Update(void)
{
	if(m_bEnable == false)
	{
		return 0;
	}

	return 0;
}

void CMagic_UI::Render(void)
{
	if(m_bEnable == false)
	{
		return;
	}

	CPlayer* pPlayer = (CPlayer*)Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Player");

	if(pPlayer->GetState() != Player_Charge)
	{
		return;	
	}
		
	m_pDevice->EndScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	Engine::TEXINFO* pTexInfo = m_p2DSprite->GetTexture(0);

	float fCX = pTexInfo->ImgInfo.Width / 2.f;
	float fCY = pTexInfo->ImgInfo.Height / 2.f;

	float fX = WINCX / 2.f;
	float fY = WINCY / 2.f;

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, fX, fY, 0.f);

	m_pSprite->SetTransform(&matTrans);
	m_pSprite->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//마법진

	pTexInfo = m_p2DSprite->GetTexture(1);

	fCX = pTexInfo->ImgInfo.Width / 2.f;
	fCY = pTexInfo->ImgInfo.Height / 2.f;

	fX = (WINCX / 2.f) + 102.f;

	D3DXMatrixTranslation(&matTrans, fX, fY, 0.f);

	m_pSprite->SetTransform(&matTrans);
	m_pSprite->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//기본

	RECT rcSize;
	rcSize.left = 0;
	rcSize.right = 50;
	rcSize.top = 0;
	rcSize.bottom = 201;

	DOUBLE	dMaxIndex = pPlayer->GetMesh()->GetPeriod();
	DOUBLE	dCurIndex = pPlayer->GetMesh()->GetTrackPosition();
	UINT	iAniIndex = pPlayer->GetAniIndex();
	float fMaxHeight = 40.f; //한칸당 최대 높이

	if(pPlayer->GetChargeComplate() == false)
	{
		float fPersent = (float)dCurIndex / (float)dMaxIndex * 100;

		fMaxHeight = fMaxHeight * fPersent / 100;

		if(iAniIndex == PlayerAniIndex_SkillCharge1)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - fMaxHeight;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge2)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - fMaxHeight - 40.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge3)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - fMaxHeight - (40.f * 2);
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge4)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - fMaxHeight - (40.f * 3);
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge5)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - fMaxHeight - (40.f * 4);
		}
	}
	else
	{
		if(iAniIndex == PlayerAniIndex_SkillCharge1)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - 40.f * 1.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge2)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - 40.f * 2.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge3)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - 40.f * 3.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge4)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - 40.f * 4.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge5)
		{
			fMaxHeight = pTexInfo->ImgInfo.Height - 40.f * 5.f;
		}
	}

	rcSize.top = (long)fMaxHeight;
	fCX = pTexInfo->ImgInfo.Width / 2.f;
	fCY = pTexInfo->ImgInfo.Height - fMaxHeight;

	fX = (WINCX / 2.f) + 102.f;
	fY = (WINCY / 2.f) + 100.5f;

	D3DXMatrixTranslation(&matTrans, fX, fY, 0.f);
	m_pSprite->SetTransform(&matTrans);
	pTexInfo = m_p2DSprite->GetTexture(3);
	m_pSprite->Draw(pTexInfo->pTexture, &rcSize, &D3DXVECTOR3(fCX, fCY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//게이지

	if(pPlayer->GetChargeComplate())
	{
		pTexInfo = m_p2DSprite->GetTexture(2);
		fCX = pTexInfo->ImgInfo.Width / 2.f;
		fCY = pTexInfo->ImgInfo.Height / 2.f;
		fX = (WINCX / 2.f) + 102.f;
		fY = WINCY / 2.f;

		D3DXMatrixTranslation(&matTrans, fX, fY, 0.f);
		m_pSprite->SetTransform(&matTrans);
		m_pSprite->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(iAniIndex == PlayerAniIndex_SkillCharge1)
		{
			pTexInfo = m_p2DSpriteICon->GetTexture(1);
			fX = (WINCX / 2.f) - 161.f;
			fY = WINCY / 2.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge2)
		{
			pTexInfo = m_p2DSpriteICon->GetTexture(3);

			fX = (WINCX / 2.f);
			fY = (WINCY / 2.f) + 159.f;
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge3)
		{
			pTexInfo = m_p2DSpriteICon->GetTexture(0);

			fX = (WINCX / 2.f) - 161.f;
			fY = (WINCY / 2.f);
		}
		else if(iAniIndex == PlayerAniIndex_SkillCharge4)
		{
			pTexInfo = m_p2DSpriteICon->GetTexture(2);

			fX = (WINCX / 2.f);
			fY = (WINCY / 2.f) + 159.f;
		}
		
		fCX = pTexInfo->ImgInfo.Width / 2.f;
		fCY = pTexInfo->ImgInfo.Height / 2.f;		

		D3DXMatrixTranslation(&matTrans, fX, fY, 0.f);
		m_pSprite->SetTransform(&matTrans);
		m_pSprite->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//차지완료

	m_pSprite->End();
	m_pDevice->BeginScene();
}

HRESULT CMagic_UI::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Texture_Magic_UI");
	m_p2DSprite = dynamic_cast<Engine::C2DSprite*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"SkillIcon");
	m_p2DSpriteICon = dynamic_cast<Engine::C2DSprite*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Texture2", pComponent));
		
	return S_OK;
}

void CMagic_UI::SetEnable(bool _Enable)
{
	m_bEnable = _Enable;
}