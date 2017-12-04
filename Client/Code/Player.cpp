#include "stdafx.h"
#include "Include.h"
#include "Player.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Magic_UI.h"
#include "Map.h"
#include "Collision.h"
#include "CollisionMgr.h"
#include "EffectMgr.h"
#include "IceBlow.h"
#include "MagicArrow.h"
#include "DynamicCamera.h"
#include "FireShock.h"
#include "IceBlast.h"
#include "DefaultEffect.h"
#include "IceDust.h"
#include "FireBolt.h"
#include "FireDust.h"
#include "ManaAmber.h"
#include "Texture.h"
#include "Kadan.h"
#include "BlinkEffect.h"
#include "DarkHurricane.h"
#include "DarkBomb.h"
#include "DarkMagic.h"
#include "SoundMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pInfo(NULL), m_pMesh(NULL), m_dwIndex(0), m_iAniIndex(0)
, m_bPush(false), m_eState(Player_Idle), m_pMapMesh(NULL), m_bMagicCreate(false)
, m_bChargeComplete(false), m_bDarkKnightTrans(false), m_fChargeAccTime(0.f)
, m_fChargeTime(0.f), m_fReturnNum(0.f), m_pManaAmber(NULL), m_eDarkKnightType(DarkKnight_None)
, m_fAccEffectTime(0.f), m_fDeleteTime(0.f), m_pSwordTexture(NULL), m_bScytheEffectCheck(false)
, m_bAttckFlag(false), m_SlideCollision(NULL), m_fSlideRadius(0.f), m_pMonster(NULL)
, m_bTransformPush(false), m_bBlink(false), m_fBlinkAccTime(0.f), m_bBlinkStart(false)
{

}

CPlayer::~CPlayer(void)
{
	Release();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pPlayer = new CPlayer(pDevice);

	if(FAILED(pPlayer->Initialize()))
	{
		Engine::Safe_Delete(pPlayer);
	}

	return pPlayer;
}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_iAniIndex = PlayerAniIndex_Idle;
	m_pMesh->SetAnimation(m_iAniIndex);
	
	m_fCharge = 0.f;
	m_fWaitTime = 0.f;

	m_bCharge = false;
	m_fRadius = 140.f;

	HRESULT hr = D3DXCreateSphere(m_pDevice, m_fRadius, 10, 10, &m_pShere, 0);
	FAILED_CHECK(hr);

	m_fSlideRadius = 15.f;
	hr = D3DXCreateSphere(m_pDevice, m_fSlideRadius, 10, 10, &m_SlideCollision, 0);
	FAILED_CHECK(hr);

	const D3DXMATRIX* matInfo = &m_pInfo->m_matWorld;
	const D3DXMATRIX* matCombine = NULL;
	Engine::CCollision* pCollision = NULL;

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Head1");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(8.f, 8.f, 9.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Calf");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(9.f, 0.f, -1.f));
	pCollision->SetScale(D3DXVECTOR3(20.f, 6.f, 6.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Calf");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(9.f, 0.f, -1.f));
	pCollision->SetScale(D3DXVECTOR3(20.f, 6.f, 6.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Foot");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(2.f, 0.f, 3.f));
	pCollision->SetScale(D3DXVECTOR3(5.f, 5.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Foot");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(2.f, 0.f, 3.f));
	pCollision->SetScale(D3DXVECTOR3(5.f, 5.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Thigh");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(10.f, 0.f, -1.f));
	pCollision->SetScale(D3DXVECTOR3(20.f, 7.f, 8.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Thigh");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(10.f, 0.f, -1.f));
	pCollision->SetScale(D3DXVECTOR3(20.f, 7.f, 8.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Pelvis");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 13.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(2.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(4.f, 4.f, 3.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Hand");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(2.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(4.f, 4.f, 3.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_ForeTwist");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(12.f, 4.f, 4.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_ForeTwist");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(12.f, 4.f, 4.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_UpperArm");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(12.f, 4.f, 4.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_UpperArm");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(12.f, 4.f, 4.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine1");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine2");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine4");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	CCollisionMgr::GetInstance()->AddData(0, &m_vecCollision);
	CCollisionMgr::GetInstance()->InitData(&m_pInfo->m_vPos, m_fRadius, Collision_Player);

	matCombine = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_RH");
	pCollision = Engine::CCollision::Create(m_pDevice, 50.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(40.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(60.f, 110.f, 5.f));
	m_vecWeaponCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_RH");
	pCollision = Engine::CCollision::Create(m_pDevice, 60.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(110.f, 10.f, 5.f));
	m_vecWeaponCollision.push_back(pCollision);

	CCollisionMgr::GetInstance()->PlayerWeapon(&m_vecWeaponCollision);

	m_pManaAmber = CManaAmber::Create(m_pDevice);

	return S_OK;
}

int CPlayer::Update(void)
{
	if(g_bCustomColor)
	{
		return 0;
	}

	CollisionCheck();
	//콜리젼 키 체크

	if(m_pMapMesh == NULL)
	{
		Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_Environment, L"Map");

		if(pObj != NULL)
		{
			m_pMapMesh = ((CMap*)pObj)->GetMesh();
			Engine::Get_Management()->Add_RenderGroup(Type_Shader, m_pManaAmber);
		}
	}

	if(m_eDarkKnightType == DarkKnight_Scythe)
	{
		DarkKnight_Scythe_Update();
	}
	else
	{
		DarkKnight_Staff_Update();
	}

	if(m_eState == Player_Hit)
	{
		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			if(m_eDarkKnightType == DarkKnight_Scythe)
			{
				if(m_iAniIndex == DarkKnightAniIndex_BigDamege_Back_Begin || m_iAniIndex == DarkKnightAniIndex_BigDamege_Front_Begin)
				{
					if(m_iAniIndex == DarkKnightAniIndex_BigDamege_Front_Begin)
					{
						m_iAniIndex = DarkKnightAniIndex_BigDamege_Front_End;
					}
					else
						m_iAniIndex = DarkKnightAniIndex_BigDamege_Back_End;
				}
				else
				{
					m_eState = Player_Idle;
					m_iAniIndex = DarkKnightAniIndex_Idle;
				}
				
				m_pMesh->SetAnimation(m_iAniIndex);	
			}
			else
			{
				if(m_iAniIndex == PlayerAniIndex_BigDamege_Back || m_iAniIndex == PlayerAniIndex_BigDamege_Front)
				{
					if(m_iAniIndex == PlayerAniIndex_BigDamege_Front)
					{
						m_iAniIndex = PlayerAniIndex_BigDamege_Front_End;
					}
					else
						m_iAniIndex = PlayerAniIndex_BigDamege_Back_End;
				}
				else
				{
					m_eState = Player_Idle;
					m_iAniIndex = PlayerAniIndex_Idle;
					m_bCharge = false;
					m_bMagicCreate = false;
					m_bChargeComplete = false;
				}

				m_pMesh->SetAnimation(m_iAniIndex);
			}
		}
	}

	SetHeight(m_pInfo->m_vDir, Engine::Get_TimeMgr()->GetTime(), 20.f);
	
	Engine::CGameObject::Update();

	size_t iSize = m_vecCollision.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		m_vecCollision[i]->Update();
	}

	iSize = m_vecWeaponCollision.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		m_vecWeaponCollision[i]->Update();
	}

	if(m_bAttckFlag)
	{
		int iIndex = CCollisionMgr::GetInstance()->PlayerToKadan();

		if(iIndex != -1)
		{
			Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Kadan");

			if(pObj != NULL)
			{
				((CKadan*)pObj)->KadanHit(iIndex, m_pInfo->m_vPos);

				m_bAttckFlag = false;
			}
		}
	}

	return 0;
}

void CPlayer::Render(void)
{
	Engine::Get_ShadowMgr()->ShadowSetConstantTable(&m_pInfo->m_matWorld, g_vLightPos);

	Engine::Get_ShadowMgr()->ShadowBegin();

	if(m_bBlink == false)
	{
		m_pMesh->Render_Shadow(Engine::Get_ShadowMgr()->GetEffect());
	}

	Engine::Get_ShadowMgr()->ShadowEnd();

	if(m_bBlink)
	{
		return;
	}
	
	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	float fTime;

	if(m_iAniIndex == PlayerAniIndex_SkillCharge1 || m_iAniIndex == PlayerAniIndex_SkillCharge2 || m_iAniIndex == PlayerAniIndex_SkillCharge3 || m_iAniIndex == PlayerAniIndex_SkillCharge4)
	{
		fTime = Engine::Get_TimeMgr()->GetTime() * 3.5f;
	}
	else if(m_eDarkKnightType == DarkKnight_Scythe && m_eState == Player_Attack)
	{
		fTime = Engine::Get_TimeMgr()->GetTime() * 2.5f;
	}
	else
		fTime = Engine::Get_TimeMgr()->GetTime() * 1.5f;

	if(m_eState != Player_ManaAmber)
	{
		m_pMesh->FrameMove(fTime);
	}	
	m_pMesh->RenderMesh(pEffect, NULL);

	if(g_bCollision_OBB || g_bCollision_Sphere || g_bCollision_BigSphere)
	{	
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		size_t iSize = m_vecCollision.size();

		for(size_t i = 0; i < iSize; ++i)
	 	{
			m_vecCollision[i]->SetBOOL(g_bCollision_OBB, g_bCollision_Sphere);
	 		m_vecCollision[i]->Render();
	 	}

		iSize = m_vecWeaponCollision.size();

		for(size_t i = 0; i < iSize; ++i)
		{
			m_vecWeaponCollision[i]->SetBOOL(g_bCollision_OBB, g_bCollision_Sphere);
			m_vecWeaponCollision[i]->Render();
		}

		if(g_bCollision_BigSphere)
		{
			D3DXVECTOR3 vPos = m_pInfo->m_vPos;
			vPos.y += 35.f;

			D3DXMATRIX matTrans;
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

			m_pDevice->SetTransform(D3DTS_WORLD, &matTrans);
			m_pShere->DrawSubset(0);
		}

		D3DXVECTOR3 vPos = m_pInfo->m_vPos;
		vPos.y += 40.f;

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		m_pDevice->SetTransform(D3DTS_WORLD, &matTrans);
		m_SlideCollision->DrawSubset(0);

	 	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	 	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if(m_eState == Player_ManaAmber)
	{
		m_pManaAmber->SetView(true);
	}
	else
		m_pManaAmber->SetView(false);

 	if(m_eDarkKnightType == DarkKnight_Scythe)
 	{
		//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


 		D3DXMATRIX matWorld;

		if(m_dequeScythEffect.size() != 0)
		{
			Engine::SWORDEFFECT* pEffect = new Engine::SWORDEFFECT[m_dequeScythEffect.size()];

			for(size_t i = 0; i < m_dequeScythEffect.size(); ++i)
			{
				pEffect[i].vPos = m_dequeScythEffect[i]->vPos;
				//pEffect[i].dwColor = m_dequeScythEffect[i]->dwColor;
				//pEffect[i].vTexUV = m_dequeScythEffect[i]->vTexUV;
				pEffect[i].vTexUV.x = 1.f - (float(i / 2) / float(m_dequeScythEffect.size() / 2 - 1));
				pEffect[i].vTexUV.y = float(i % 2);
			}

			D3DXMatrixIdentity(&matWorld);	
			m_pSwordTexture->Render(0, 3);
			m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
			m_pDevice->SetFVF(Engine::SWORDFVF);
			m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_dequeScythEffect.size() - 2, pEffect, sizeof(Engine::SWORDEFFECT));

			Engine::Safe_Delete_Array(pEffect);
		}

		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
 	}
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));
	
	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Player");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_pMesh->SetOwner(Collision_Player);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Player_Mesh", pComponent));

	//DarkKnight_Staff
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"DarkKnight_Staff");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	((Engine::CDynamicMesh*)pComponent)->SetOwner(Collision_Player);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"DarkKnight_Staff", pComponent));

	//DarkKnight_Scythe
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"DarkKnight_Scythe");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	((Engine::CDynamicMesh*)pComponent)->SetOwner(Collision_Player);
	m_pMatWeapon = ((Engine::CDynamicMesh*)pComponent)->FindFrame("ValveBiped_Anim_Attachment_RH");
	m_pMatWeapon2 = ((Engine::CDynamicMesh*)pComponent)->FindFrame("ValveBiped_Anim_Attachment_Bolt");
	m_pMatWeapon3 = ((Engine::CDynamicMesh*)pComponent)->FindFrame("ValveBiped_Anim_Attachment_ragdoll");

	//m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"DarkKnight_Scythe", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));

	m_eDarkKnightType = DarkKnight_None;

	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Scythe_Effect");
	m_pSwordTexture = (Engine::CTexture*)pComponent;
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));
	
	return S_OK;
}

HRESULT CPlayer::SetConstantTable(void)
{
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	FAILED_CHECK_RETURN(pEffect, E_FAIL);

	D3DXMATRIX matView, matProj, matInverWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInverWorld, NULL, &m_pInfo->m_matWorld);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetMatrix("g_matInverWorld", &matInverWorld);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffus", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3 vCameraPos(matView._41, matView._42, matView._43);

	pEffect->SetVector("g_vCameraPos", &D3DXVECTOR4(vCameraPos, 0.f));

	return S_OK;
}

bool CPlayer::KeyCheck(void)
{
	if(m_eState == Player_Hit)
	{
		return false;
	}

	bool bCheck = false;
	bool bMoveCheck = false;
	
	DWORD dwKey = 0;

	float fTime = Engine::Get_TimeMgr()->GetTime();

	D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_Z) & 0x80)
	{
		if(m_bTransformPush)
		{
			return false;
		}

		m_bTransformPush = true;

		if(m_bDarkKnightTrans)
		{
			return false;
		}

		if(m_eDarkKnightType == DarkKnight_None)
		{
			CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_effect_darkknight.wav");

			m_eState = Player_Transform;
			m_bDarkKnightTrans = true;
			m_iAniIndex = PlayerAniIndex_DarkKnight_Transform1;
			m_pMesh->SetAnimation(m_iAniIndex);

			Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Camera");
			((CDynamicCamera*)pObj)->SetDarkKnight(true);

			const D3DXMATRIX* pMatCombine = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_EyeTarget");

			((CDynamicCamera*)pObj)->SetMatrix(&m_pInfo->m_matWorld, pMatCombine);
		}
		else if(m_eDarkKnightType == DarkKnight_Staff)
		{
			if(m_iAniIndex == PlayerAniIndex_Idle)
			{
				Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)GetComponent(L"DarkKnight_Scythe");
				pMesh->SetAnimation(DarkKnightAniIndex_Idle);			
				pMesh->SetTrackPos(m_pMesh->GetTrackPosition());
				m_pMesh = pMesh;
				m_eDarkKnightType = DarkKnight_Scythe;
				Collision_Sync();
			}
		}
	}
	else
		m_bTransformPush = false;
// 
// 	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_L) & 0x80)
// 	{
// 		if(m_bTestPush)
// 		{
// 			return false;
// 		}
// 
// 		m_bTestPush = true;
// 
// 		CIceBlow* pSkill = CIceBlow::Create(m_pDevice);
// 		D3DXVECTOR3 vSkillPos = m_pInfo->m_vPos;
// 		D3DXVECTOR3 vRight(1.f, 0.f, 0.f);
// 		D3DXVec3TransformNormal(&vRight, &vRight, &m_pInfo->m_matWorld);
// 		vSkillPos += vRight * 50.f;
// 		pSkill->SetInfo(vSkillPos);
// 		pSkill->SetAngel(Angle_Y, m_pInfo->m_fAngle[Angle_Y] - 90.f);
// 		Engine::Get_SkillMgr()->AddSkill(pSkill);
// 	}
// 	else
// 		m_bTestPush = false;
	
	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_W) & 0x80)
	{
		if(m_bChargeComplete)
		{
			
		}
		else if(m_bCharge == false && m_eState != Player_ManaAmber)
		{
			dwKey |= KEY_UP;
			bMoveCheck = true;
		}
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_S) & 0x80)
	{
		if(m_bChargeComplete)
		{
			if(m_iAniIndex == PlayerAniIndex_SkillCharge4)
			{
				m_eState = Player_Attack;
				SetState(PlayerAniIndex_Magic_IceBlow);
				m_bChargeComplete = false;
				m_fChargeAccTime = 0.f;
				Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_UI, L"Magic_UI");
				((CMagic_UI*)pObj)->SetEnable(false);

				CIceBlow* pSkill = CIceBlow::Create(m_pDevice);
				D3DXVECTOR3 vSkillPos = m_pInfo->m_vPos;
				D3DXVECTOR3 vRight(1.f, 0.f, 0.f);
				D3DXVec3TransformNormal(&vRight, &vRight, &m_pInfo->m_matWorld);
				vSkillPos += vRight * 50.f;
				pSkill->SetInfo(vSkillPos);
				pSkill->SetAngel(Angle_Y, m_pInfo->m_fAngle[Angle_Y] - 90.f);
				Engine::Get_SkillMgr()->AddSkill(pSkill);
			}
			else if(m_iAniIndex == PlayerAniIndex_SkillCharge2)
			{
				m_eState = Player_Attack;
				SetState(PlayerAniIndex_Magic_IceSpaer);

				if(m_bMagicCreate == false)
				{
					m_bMagicCreate = true;
				}
			}
		}
		else if(m_bCharge == false && m_eState != Player_ManaAmber)
		{
			dwKey |= KEY_DOWN;
			bMoveCheck = true;
		}		
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_A) & 0x80)
	{
		if(m_bChargeComplete)
		{
			if(m_iAniIndex == PlayerAniIndex_SkillCharge3)
			{
				m_eState = Player_Attack;
				SetState(PlayerAniIndex_Magic_FireShock);

				if(m_bMagicCreate == false)
				{
					m_bMagicCreate = true;
				}
			}
			else if(m_iAniIndex == PlayerAniIndex_SkillCharge1)
			{
				m_eState = Player_Attack;
				SetState(PlayerAniIndex_Magic_FireBolt);

				if(m_bMagicCreate == false)
				{
					m_bMagicCreate = true;
				}
			}
		}
		else if(m_bCharge == false && m_eState != Player_ManaAmber)
		{
			dwKey |= KEY_LEFT;
			bMoveCheck = true;
		}
	}
	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_D) & 0x80)
	{
		if(m_bChargeComplete)
		{
			
		}
		else if(m_bCharge == false && m_eState != Player_ManaAmber)
		{
			dwKey |= KEY_RIGHT;
			bMoveCheck = true;
		}	
	}

	if(Engine::Get_InputMgr()->GetDIMouseState(DIM_RButton) & 0x80)
	{
		if(bMoveCheck || m_eState == Player_Attack)
		{
			return false;
		}
			
		dwKey = 0;
		dwKey |= KEY_UP;

		SetDir(KEY_UP);

		m_eState = Player_Charge;

		if(m_bCharge == false)
		{
			SetState(PlayerAniIndex_SkillCharge1);
			m_fChargeAccTime = 0.f;
			Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_UI, L"Magic_UI");
			((CMagic_UI*)pObj)->SetEnable(true);
			m_fReturnNum = 0.f;
			m_fChargeTime = 0.4f;
			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_charge_01.wav");
		}

		m_bCharge = true;
		bCheck = true;
	}
	else
	{
		m_bCharge = false;
	}

	if(Engine::Get_InputMgr()->GetDIMouseState(DIM_Lbutton) & 0x80)
	{
		if(m_bPush)
		{
			return false;
		}

		if(bMoveCheck)
		{
			bMoveCheck = false;
		}

		if(PlayerAniIndex_Magic_MagicArrow != m_iAniIndex)
		{
			if(m_bMagicCreate == false)
			{
				m_bMagicCreate = true;
			}
		}

		dwKey = 0;
		dwKey |= KEY_UP;

		SetDir(dwKey);

		m_eState = Player_Attack;

		SetState(PlayerAniIndex_Magic_MagicArrow);
		
		m_bPush = true;
	}
	else
		m_bPush = false;

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_SPACE) & 0x80)
	{
		if(bMoveCheck)
		{
			//이건 나중에....
		}
		else
		{
			if(bCheck == false && m_eState != Player_ManaAmber)
			{
				m_ePrevState = m_eState;
				m_eState = Player_ManaAmber;

				m_pManaAmber->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 40.f, m_pInfo->m_vPos.z));

				CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_mana_amber_begin.wav");
			}
		}
	}
	else
	{
		if(m_eState == Player_ManaAmber)
		{
			m_eState = m_ePrevState;
			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_mana_amber_end.wav");
		}
	}

	if(m_eState != Player_Attack)
	{
		if(bMoveCheck)
		{	
			bCheck = true;
			m_eState = Player_Move;
			SetState(PlayerAniIndex_Move);
			SetDir(dwKey);

			if(!WallCheck() && !SetHeight(m_pInfo->m_vDir, fTime, 150.f))
			{
				CollisionSphereCheck(m_pInfo->m_vPos, m_pInfo->m_vDir, fTime, 150.f);

				m_pInfo->m_vPos += m_pInfo->m_vDir * 150.f * fTime;	
			}
		}
	}

	return bCheck;
}

void CPlayer::SetState(ePlayerAniIndex _AniIndex)
{
	if(m_iAniIndex == _AniIndex)
	{
		if(m_iAniIndex == PlayerAniIndex_Magic_MagicArrow)
		{
			if(m_pMesh->GetTrackPosition() >= 1.6)
			{	
				if(m_bMagicCreate == false)
				{
					m_bMagicCreate = true;
				}

				m_pMesh->SetTrackPos(0.6);
			}
		}
		return;
	}

	m_iAniIndex = _AniIndex;

	m_pMesh->SetAnimation(m_iAniIndex);
}

void CPlayer::SetDir(DWORD _dwKey)
{
	D3DXMATRIX matView;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3 vCameraPos(matView._41, matView._42, matView._43);

	D3DXVECTOR3 vCameraDir = m_pInfo->m_vPos - vCameraPos;

	vCameraDir.y = 0.f;

	D3DXVec3Normalize(&vCameraDir, &vCameraDir);

	D3DXMATRIX matRotY;

	if(_dwKey & KEY_UP)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(0.f));
	}
	if(_dwKey & KEY_DOWN)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(180.f));
	}
	if(_dwKey & KEY_LEFT)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(270.f));
	}
	if(_dwKey & KEY_RIGHT)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(90.f));
	}

	if(_dwKey & KEY_RIGHT && _dwKey & KEY_UP)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(45.f));
	}
	if(_dwKey & KEY_LEFT && _dwKey & KEY_UP)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(315.f));
	}
	if(_dwKey & KEY_DOWN && _dwKey & KEY_LEFT)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(225.f));
	}
	if(_dwKey & KEY_DOWN && _dwKey & KEY_RIGHT)
	{
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(135.f));
	}

	//요건 키 입력이 있을때
	D3DXVec3TransformNormal(&vCameraDir, &vCameraDir, &matRotY);

	float fAngle = D3DXVec3Dot(&g_vLook, &vCameraDir);

	fAngle = acosf(fAngle);

	D3DXVECTOR3 vX(1.f, 0.f, 0.f);
 
 	float fCameraAngle = D3DXVec3Dot(&vX, &vCameraDir);
 
 	fCameraAngle = acosf(fCameraAngle);
 
 	if(D3DXToDegree(fCameraAngle) >= 90.f)
 	{
 		fAngle = 2 * D3DX_PI - fAngle;
 	}
  	if(fAngle >= 0.f && fAngle <= 360.f)
  	{
  		m_pInfo->m_fAngle[Angle_Y] = fAngle;
  	}
  	else
  	{
  		m_pInfo->m_fAngle[Angle_Y] = 0.f;
  	}

	m_pInfo->Update();

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
}

bool CPlayer::SetHeight(D3DXVECTOR3 _vDir, float fTime, float fSpeed)
{
	DWORD dwIndex, dwCnt;
	float fU, fV, fDist;

	D3DXVECTOR3 vPos = m_pInfo->m_vPos;
	vPos += _vDir * fTime * fSpeed;

	D3DXVECTOR3 vRayDir(0.f, -1.f, 0.f);
	D3DXVECTOR3 vRayPos(vPos.x, vPos.y + 20.f, vPos.z);

	BOOL bCheck = FALSE;

	LPD3DXBUFFER pSubSet = m_pMapMesh->GetSubSet();

	D3DXIntersect(m_pMapMesh->GetMesh(), &vRayPos, &vRayDir, &bCheck, &dwIndex, &fU, &fV, &fDist, &pSubSet, &dwCnt);

	if(bCheck == TRUE)
	{
		if(fDist <= 40.f)
		{
			D3DXVECTOR3 vPoint = vRayPos + vRayDir * fDist;
			m_pInfo->m_vPos.y = vPoint.y;

			return false;
		}
		else
			return true;
	}

	return true;
}

bool CPlayer::WallCheck(void)
{	
	DWORD dwIndex, dwCnt;
	float fU, fV, fDist;

	D3DXVECTOR3 vRayDir = m_pInfo->m_vDir;
	D3DXVECTOR3 vRayPos(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 15.f, m_pInfo->m_vPos.z);

	BOOL bCheck = FALSE;

	LPD3DXBUFFER pSubSet = m_pMapMesh->GetSubSet();

	D3DXIntersect(m_pMapMesh->GetMesh(), &vRayPos, &vRayDir, &bCheck, &dwIndex, &fU, &fV, &fDist, &pSubSet, &dwCnt);

	if(bCheck == TRUE)
	{
		if(fDist <= 15.f)
		{
			return true;
		}
	}
	
	return false;
}

void CPlayer::Release(void)
{
	size_t iSize = m_vecCollision.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		Engine::Safe_Delete(m_vecCollision[i]);
	}
	m_vecCollision.clear();

	Engine::Safe_Delete(m_pManaAmber);

	for(size_t i = 0; i < m_dequeScythEffect.size(); ++i)
	{
		Engine::Safe_Delete(m_dequeScythEffect[i]);
	}
	m_dequeScythEffect.clear();

	for(size_t i = 0; i < m_vecWeaponCollision.size(); ++i)
	{
		Engine::Safe_Delete(m_vecWeaponCollision[i]);
	}
	m_vecWeaponCollision.clear();
}

void CPlayer::SetCustomColor(D3DXVECTOR4 vCustomColor, eCustomParts _Parts, eCustomColor _Color)
{
	m_pMesh->SetCustomRGB(vCustomColor, _Color, _Parts);
}

ePlayerState CPlayer::GetState(void)
{
	return m_eState;
}

bool CPlayer::GetChargeComplate(void)
{
	return m_bChargeComplete;
}

Engine::CDynamicMesh* CPlayer::GetMesh(void)
{
	return m_pMesh;
}

UINT CPlayer::GetAniIndex(void)
{
	return m_iAniIndex;
}

int CPlayer::DarkKnight_Staff_Update(void)
{
	bool bCheck = KeyCheck();

	if(m_eState == Player_Move)
	{
		if(bCheck == false)
		{	
			m_eState = Player_Idle;
			m_iAniIndex = PlayerAniIndex_Idle;
			m_pMesh->SetAnimation(m_iAniIndex);
			m_pMesh->SetTrackPos(0.0);
		}
	}
	else if(m_eState == Player_Attack)
	{
		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_iAniIndex = PlayerAniIndex_Idle;
			m_eState = Player_Idle;

			m_pMesh->SetAnimation(m_iAniIndex);
			m_pMesh->SetTrackPos(0.0);
		}
	}
	else if(m_eState == Player_ManaAmber)
	{
		m_pManaAmber->Update();
	}
	else if(m_eState == Player_Charge)
	{
		if(bCheck == true && m_bCharge == true)
		{
			DOUBLE dChargeTime = m_pMesh->GetTrackPosition();

			if(dChargeTime >= m_pMesh->GetPeriod() - 0.2)
			{
				m_bChargeComplete = true;

				if(m_fChargeAccTime >= m_fChargeTime)
				{
					switch(m_iAniIndex)
					{
					case PlayerAniIndex_SkillCharge1:
						m_iAniIndex = PlayerAniIndex_SkillCharge2;
						m_fReturnNum = 1.f;
						m_fChargeTime = 0.4f;
						CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_charge_02.wav");
						break;

					case PlayerAniIndex_SkillCharge2:
						m_iAniIndex = PlayerAniIndex_SkillCharge3;
						m_fReturnNum = 1.f;
						m_fChargeTime = 0.4f;
						CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_charge_03.wav");
						break;

					case PlayerAniIndex_SkillCharge3:
						m_iAniIndex = PlayerAniIndex_SkillCharge4;
						m_fReturnNum = 0.8f;
						m_fChargeTime = 0.4f;
						CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_charge_04.wav");
						break;

					case PlayerAniIndex_SkillCharge4:
						m_iAniIndex = PlayerAniIndex_SkillCharge5;
						break;

					case PlayerAniIndex_SkillCharge5:
						break;
					}
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);

					m_fChargeAccTime = 0.f;
					m_bChargeComplete = false;
				}
				else
				{
					DOUBLE dMaxFrame = m_pMesh->GetPeriod();
					//m_pMesh->SetAnimation(m_iAniIndex);
					//m_pMesh->SetTrackPos(dMaxFrame - m_fReturnNum);
					m_pMesh->SetTrackPos(dMaxFrame - 0.2);
					m_bChargeComplete = true;
				}
			}
		}
		else
		{
			if(m_bChargeComplete)
			{
				if(m_fChargeAccTime >= m_fChargeTime)
				{
					m_eState = Player_Idle;
					m_iAniIndex = PlayerAniIndex_Idle;
					m_bCharge = false;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_bChargeComplete = false;
					m_fChargeAccTime = 0.f;

					Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_UI, L"Magic_UI");
					((CMagic_UI*)pObj)->SetEnable(false);
				}
				else
				{
					if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
					{
						m_pMesh->SetAnimation(m_iAniIndex);
						m_pMesh->SetTrackPos(m_pMesh->GetPeriod() - 0.2/*m_fReturnNum*/);
					}
				}
			}
			else
			{
				m_eState = Player_Idle;
				m_iAniIndex = PlayerAniIndex_Idle;
				m_bCharge = false;
				m_pMesh->SetAnimation(m_iAniIndex);
				m_bChargeComplete = false;
				m_fChargeAccTime = 0.f;

				Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_UI, L"Magic_UI");
				((CMagic_UI*)pObj)->SetEnable(false);
			}	
		}
	}

	if(m_bChargeComplete)
	{
		m_fChargeAccTime += Engine::Get_TimeMgr()->GetTime();
	}

	if(m_iAniIndex == PlayerAniIndex_Magic_MagicArrow && m_bMagicCreate)
	{
		if(m_pMesh->GetTrackPosition() >= 0.6)
		{
			D3DXVECTOR3 vPos = m_pInfo->m_vPos;
			vPos.y += 50.f;
			vPos += m_pInfo->m_vDir * 50.f;
			
		
			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_arrow_fire.wav");

			CDefaultEffect* pEffect = CDefaultEffect::Create(m_pDevice, vPos, D3DXVECTOR3(35.f, 35.f, 35.f), D3DXVECTOR3(0.f, 0.f, 0.f), 0.5f, 0.f, 0.f, 0.f, 0.f, Angle_End, false, D3DXVECTOR3(1.f, 1.f, 1.f), 20.f, L"Magic_Arrow", 2);
			CEffectMgr::GetInstance()->AddEffect(pEffect);

			pEffect = CDefaultEffect::Create(m_pDevice, vPos, D3DXVECTOR3(32.f, 32.f, 32.f), D3DXVECTOR3(0.f, 0.f, 0.f), 0.5f, 0.f, 0.f, 0.f, 0.f, Angle_End, false, D3DXVECTOR3(1.f, 1.f, 1.f), 20.f, L"Magic_Arrow", 2);
			CEffectMgr::GetInstance()->AddEffect(pEffect);

			CMagicArrow* pArrow = CMagicArrow::Create(m_pDevice);
			pArrow->SetInfo(vPos);
			pArrow->SetTime(1.f);
			pArrow->SetDir(m_pInfo->m_vDir);

			Engine::Get_SkillMgr()->AddSkill(pArrow);
			m_bMagicCreate = false;
		}
	}
	else if(m_iAniIndex == PlayerAniIndex_Magic_FireShock && m_bMagicCreate)
	{
		if(m_pMesh->GetTrackPosition() >= 1.9)
		{
			D3DXVECTOR3 vPos = m_pInfo->m_vPos;
			vPos.y += 1.f;
			vPos += m_pInfo->m_vDir * 150.f;

			CFireShock* pSkill = CFireShock::Create(m_pDevice);

			pSkill->GetInfo()->m_vPos = vPos;
			Engine::Get_SkillMgr()->AddSkill(pSkill);
			m_bMagicCreate = false;

			m_bChargeComplete = false;
			m_fChargeAccTime = 0.f;
			
				CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack3.wav");
			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_fire_shock_during_1.wav");			
		}
	}
	else if(m_iAniIndex == PlayerAniIndex_Magic_FireBolt && m_bMagicCreate)
	{
		if(m_pMesh->GetTrackPosition() >= 1.9)
		{
			D3DXVECTOR3 vPos = m_pInfo->m_vPos;
			vPos.y += 50.f;
			vPos += m_pInfo->m_vDir * 50.f;

			CFireBolt* pSkill = CFireBolt::Create(m_pDevice);
			pSkill->GetInfo()->m_vPos = vPos;
			pSkill->GetInfo()->m_vDir = m_pInfo->m_vDir;
			Engine::Get_SkillMgr()->AddSkill(pSkill);

			CFireDust* pEffect = CFireDust::Create(m_pDevice);
			pEffect->SetInfo(vPos);
			CEffectMgr::GetInstance()->AddEffect(pEffect);

			m_bMagicCreate = false;
			m_bChargeComplete = false;
			m_fChargeAccTime = 0.f;
			CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack2.wav");	
			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_firebolt_fire.wav");	
		}
	}
	else if(m_iAniIndex == PlayerAniIndex_Magic_IceSpaer && m_bMagicCreate)
	{	
		if(m_pMesh->GetTrackPosition() >= 1.9)
		{
			D3DXVECTOR3 vPos = m_pInfo->m_vPos;
			vPos.y += 50.f;
			vPos += m_pInfo->m_vDir * 50.f;

			CIceBlast* pSkill = CIceBlast::Create(m_pDevice);
			pSkill->GetInfo()->m_vPos = vPos;
			pSkill->GetInfo()->m_vDir = m_pInfo->m_vDir;
			pSkill->GetInfo()->m_fAngle[Angle_Y] = m_pInfo->m_fAngle[Angle_Y];
			Engine::Get_SkillMgr()->AddSkill(pSkill);

			CIceDust* pEffect = CIceDust::Create(m_pDevice);
			pEffect->SetInfo(vPos);

			CEffectMgr::GetInstance()->AddEffect(pEffect);

			m_bMagicCreate = false;
			m_bChargeComplete = false;
			m_fChargeAccTime = 0.f;
			CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack3.wav");
			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_ice_spear.wav");
		}		
		//아이스 스피어.
	}

	if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
	{
		if(m_iAniIndex == PlayerAniIndex_DarkKnight_Transform1)
		{	
			Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Camera");
			((CDynamicCamera*)pObj)->SetDarkKnight(false);

			m_pMesh = (Engine::CDynamicMesh*)GetComponent(L"DarkKnight_Staff");
			m_iAniIndex = PlayerAniIndex_Idle;
			m_pMesh->SetAnimation(m_iAniIndex);
			m_bDarkKnightTrans = false;
			Collision_Sync();
			m_eDarkKnightType = DarkKnight_Staff;
		}
	}

	return 0;	
}

int CPlayer::DarkKnight_Scythe_Update(void)
{
	bool bCheck = ScytheKeyCheck();

	float fTime = Engine::Get_TimeMgr()->GetTime();

	if(bCheck == false)
	{
		if(m_eState == Player_Move || m_eState == Player_Dash)
		{
			if(m_iAniIndex != DarkKnightAniIndex_Idle)
			{
				m_eState = Player_Idle;
				m_iAniIndex = DarkKnightAniIndex_Idle;
				m_pMesh->SetAnimation(m_iAniIndex);
				m_pMesh->SetTrackPos(0.0);
				m_bAttckFlag = false;
			}
		}
	}

	if(m_eState == Player_Blink)
	{
		if(m_bBlink == false && m_bBlinkStart)
		{
			if(m_pMesh->GetTrackPosition() >= 0.6)
			{
				m_bBlink = true;
				m_bBlinkStart = false;

				m_fBlinkAccTime = 0.f;

				CBlinkEffct* pEffect = CBlinkEffct::Create(m_pDevice);
				pEffect->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 10.f, m_pInfo->m_vPos.z));
				CEffectMgr::GetInstance()->AddEffect(pEffect);

				m_bBlinkEffect = false;

				CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_blink_appear.wav");

				Engine::Get_Management()->m_pRenderer->m_bBlur = true;
			}
		}
		else
		{
			m_fBlinkAccTime += fTime;

			if(m_fBlinkAccTime >= 0.5f)
			{
				m_fBlinkAccTime = 0.f;
				m_bBlink = false;

				if(m_bBlinkEffect == false)
				{
					CBlinkEffct* pEffect = CBlinkEffct::Create(m_pDevice);
					pEffect->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 10.f, m_pInfo->m_vPos.z));
					CEffectMgr::GetInstance()->AddEffect(pEffect);

					m_bBlinkEffect = true;

					CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_blink_disappear.wav");

					Engine::Get_Management()->m_pRenderer->m_bBlur = false;
				}				
			}
		}

		if(m_bBlink)
		{
			SetDir(m_dwKey);
			//이동을 하자....
			if(!WallCheck() && !SetHeight(m_pInfo->m_vDir, fTime, 450.f))
			{
				CollisionSphereCheck(m_pInfo->m_vPos, m_pInfo->m_vDir, fTime, 450.f);
				m_pInfo->m_vPos += m_pInfo->m_vDir * 450.f * fTime;
			}
		}

		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_eState = Player_Idle;
			m_iAniIndex = DarkKnightAniIndex_Idle;
			m_pMesh->SetAnimation(m_iAniIndex);
			m_pMesh->SetTrackPos(0.0);
		}
	}

	if(m_dequeScythEffect.size() >= 50 || m_eState != Player_Attack)
	{
		if(m_dequeScythEffect.size() != 0)
		{
			Engine::Safe_Delete(m_dequeScythEffect[0]);
			Engine::Safe_Delete(m_dequeScythEffect[1]);
			m_dequeScythEffect.pop_front();
			m_dequeScythEffect.pop_front();
		}
	}

	if(m_eState == Player_Attack)
	{
		m_fAccEffectTime += fTime;

		if(m_fAccEffectTime >= 0.01f)
		{
			m_fAccEffectTime = 0.f;
			
			Engine::SWORDEFFECT* pEffect = new Engine::SWORDEFFECT;
			pEffect->vPos = D3DXVECTOR3(60.f, 0.f, 0.f);
			D3DXMATRIX matWorld = (*m_pMatWeapon) * m_pInfo->m_matWorld;
			D3DXVec3TransformCoord(&pEffect->vPos, &pEffect->vPos, &matWorld);
			m_dequeScythEffect.push_back(pEffect);

			pEffect = new Engine::SWORDEFFECT;
			pEffect->vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
			matWorld = (*m_pMatWeapon) * m_pInfo->m_matWorld;
			D3DXVec3TransformCoord(&pEffect->vPos, &pEffect->vPos, &matWorld);
			m_dequeScythEffect.push_back(pEffect);			
		}
			
		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_eState = Player_Idle;
			m_iAniIndex = DarkKnightAniIndex_Idle;
			m_pMesh->SetAnimation(m_iAniIndex);
			m_pMesh->SetTrackPos(0.0);
			m_bAttckFlag = false;
		}

		switch(m_iAniIndex)
		{
		case DarkKnightAniIndex_Attack1:
			if(m_pMesh->GetTrackPosition() <= 0.4)
			{
				SetDir(m_dwKey);

				if(!WallCheck() && !SetHeight(m_pInfo->m_vDir, fTime, 150.f))
				{
					m_pInfo->m_vPos += m_pInfo->m_vDir * 150.f * fTime;
				}
			}		
			break;

		case DarkKnightAniIndex_Attack2:
			if(m_pMesh->GetTrackPosition() >= 0.49 && m_pMesh->GetTrackPosition() <= 1.0)
			{
				SetDir(m_dwKey);

				if(!WallCheck() && !SetHeight(m_pInfo->m_vDir, fTime, 150.f))
				{
					m_pInfo->m_vPos += m_pInfo->m_vDir * 150.f * fTime;
				}
			}
			break;

		case DarkKnightAniIndex_Attack3:
			if(m_pMesh->GetTrackPosition() >= 0.60 && m_pMesh->GetTrackPosition() <= 1.3)
			{
				SetDir(m_dwKey);

				if(!WallCheck() && !SetHeight(m_pInfo->m_vDir, fTime, 150.f))
				{
					m_pInfo->m_vPos += m_pInfo->m_vDir * 150.f * fTime;
				}
			}
			break;
		}
	}

	return 0;
}

bool CPlayer::ScytheKeyCheck(void)
{
	bool bCheck = false;
	bool bMoveCheck = false;

	if(m_eState == Player_Hit)
	{
		return false;
	}

	m_dwKey = 0;

	float fTime = Engine::Get_TimeMgr()->GetTime();

	D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_Z) & 0x80)
	{
		if(m_bTransformPush)
		{
			return false;
		}

		m_bTransformPush = true;

		if(m_eDarkKnightType == DarkKnight_Scythe)
		{
			if(m_iAniIndex == PlayerAniIndex_Idle)
			{
				Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)GetComponent(L"DarkKnight_Staff");
				pMesh->SetAnimation(PlayerAniIndex_Idle);			
				pMesh->SetTrackPos(m_pMesh->GetTrackPosition());
				m_pMesh = pMesh;
				m_eDarkKnightType = DarkKnight_Staff;
				Collision_Sync();
			}
		}
	}
	else
		m_bTransformPush = false;

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_W) & 0x80)
	{
		m_dwKey |= KEY_UP;
		bMoveCheck = true;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_S) & 0x80)
	{
		m_dwKey |= KEY_DOWN;
		bMoveCheck = true;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_A) & 0x80)
	{
		m_dwKey |= KEY_LEFT;
		bMoveCheck = true;
	}
	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_D) & 0x80)
	{	
		m_dwKey |= KEY_RIGHT;
		bMoveCheck = true;
	}

	if(m_eState == Player_Attack)
	{
		bMoveCheck = false;
	}

	if(Engine::Get_InputMgr()->GetDIMouseState(DIM_RButton) & 0x80)
	{
		if(m_eState == Player_Blink)
		{
			return false;
		}

		if(m_bStrongPush)
		{
			return false;
		}

		if(bMoveCheck)
		{
			bMoveCheck = false;
		}

		if(m_eState != Player_Attack)
		{
			m_eState = Player_Attack;
			m_iAniIndex = DarkKnightAniIndex_StrongAttack0;
			m_pMesh->SetAnimation(m_iAniIndex);
			m_bAttckFlag = true;

			CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_strong_01.wav");
			CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack1.wav");
		}
		else if(m_eState == Player_Attack)
		{
			switch(m_iAniIndex)
			{
			case DarkKnightAniIndex_Attack1:
				if(m_pMesh->GetTrackPosition() >= 0.5 && m_pMesh->GetTrackPosition() <= 1.1)
				{
					m_iAniIndex = DarkKnightAniIndex_StrongAttack1_a;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;
					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_strong_01.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack1.wav");
				}
				break;

			case DarkKnightAniIndex_Attack2:
				if(m_pMesh->GetTrackPosition() >= 1.3 && m_pMesh->GetTrackPosition() <= 1.8)
				{
					m_iAniIndex = DarkKnightAniIndex_StrongAttack2;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;

					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_strong_02.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack2.wav");
				}
				break;

			case DarkKnightAniIndex_Attack3:
				if(m_pMesh->GetTrackPosition() >= 1.7 && m_pMesh->GetTrackPosition() <= 2.2)
				{
					m_iAniIndex = DarkKnightAniIndex_StrongAttack3;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;

					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_strong_03.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack3.wav");
				}
				break;

			case DarkKnightAniIndex_Attack4:
				if(m_pMesh->GetTrackPosition() >= 2.5 && m_pMesh->GetTrackPosition() <= 3.0)
				{
					m_iAniIndex = DarkKnightAniIndex_StrongAttack4;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;

					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_strong_04.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack4.wav");
				}
				break;
			}
		}

		bCheck = true;
		m_bStrongPush = true;
	}
	else
		m_bStrongPush = false;

	if(Engine::Get_InputMgr()->GetDIMouseState(DIM_Lbutton) & 0x80)
	{
		if(m_eState == Player_Blink)
		{
			return false;
		}

		if(m_bPush)
		{
			return false;
		}

		if(bMoveCheck)
		{
			bMoveCheck = false;
		}
		
		//공격으로 스테이터스 변경을 했을때 이동.
		
		//최초 1회와 2회째공격의 차이점??

		if(m_eState != Player_Attack)
		{
			m_eState = Player_Attack;
			m_iAniIndex = DarkKnightAniIndex_Attack1;
			m_pMesh->SetAnimation(m_iAniIndex);

			m_bAttckFlag = true;

			CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_01.wav");
			CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_attack1.wav");
			//1타의 구분...
		}
		else if(m_eState == Player_Attack)
		{
			switch(m_iAniIndex)
			{
			case DarkKnightAniIndex_Attack1:
				if(m_pMesh->GetTrackPosition() >= 0.5 && m_pMesh->GetTrackPosition() <= 1.1)
				{
					m_iAniIndex = DarkKnightAniIndex_Attack2;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;
					
					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_02.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_attack2.wav");
					
				}
				break;

			case DarkKnightAniIndex_Attack2:
				if(m_pMesh->GetTrackPosition() >= 1.3 && m_pMesh->GetTrackPosition() <= 1.8)
				{
					m_iAniIndex = DarkKnightAniIndex_Attack3;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;

					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_03.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_attack3.wav");
					
				}
				break;
				
			case DarkKnightAniIndex_Attack3:
				if(m_pMesh->GetTrackPosition() >= 1.7 && m_pMesh->GetTrackPosition() <= 2.2)
				{
					m_iAniIndex = DarkKnightAniIndex_Attack4;
					m_pMesh->SetAnimation(m_iAniIndex);
					m_pMesh->SetTrackPos(0.0);
					m_bAttckFlag = true;

					CSoundMgr::GetInstance()->PlayPlayerSound(L"evy_swing_scythe_attack_04.wav");
					CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_attack4.wav");

				}		
				break;
			}
		}

		bCheck = true;
		m_bPush = true;
	}
	else
		m_bPush = false;

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_SPACE) & 0x80)
	{
 		if(m_eState != Player_Blink && m_eState != Player_Attack)
 		{
			bMoveCheck = false;

			m_eState = Player_Blink;

			if(m_dwKey |= KEY_UP)
			{
				m_iAniIndex = DarkKnightAniIndex_Blink_N;
			}
			else if(m_dwKey |= KEY_DOWN)
			{
				m_iAniIndex = DarkKnightAniIndex_Blink_S;
			}
			else
				m_iAniIndex = DarkKnightAniIndex_Blink_S;

			SetDir(m_dwKey);

			m_bBlinkStart = true;

			m_pMesh->SetAnimation(m_iAniIndex);	
 		}
	}

	if(m_eState == Player_Blink)
	{
		bMoveCheck = false;
	}

	if(m_eState != Player_Attack)
	{
		if(bMoveCheck)
		{	
			bCheck = true;
			m_eState = Player_Move;
			SetState(PlayerAniIndex_Move);
			SetDir(m_dwKey);

			if(!WallCheck() && !SetHeight(m_pInfo->m_vDir, fTime, 150.f))
			{
				CollisionSphereCheck(m_pInfo->m_vPos, m_pInfo->m_vDir, fTime, 150.f);

				m_pInfo->m_vPos += m_pInfo->m_vDir * 150.f * fTime;

				CSoundMgr::GetInstance()->PlayPlayerMoveSound(L"Action_Dash.wav");
			}
		}
	}//이동할때

	return bCheck;
}

void CPlayer::Collision_Sync(void)
{
	const D3DXMATRIX* matCombine = NULL;

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Head1");
	m_vecCollision[0]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Calf");
	m_vecCollision[1]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Calf");
	m_vecCollision[2]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Foot");
	m_vecCollision[3]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Foot");
	m_vecCollision[4]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Thigh");
	m_vecCollision[5]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Thigh");
	m_vecCollision[6]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Pelvis");
	m_vecCollision[7]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");
	m_vecCollision[8]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Hand");
	m_vecCollision[9]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_ForeTwist");
	m_vecCollision[10]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_ForeTwist");
	m_vecCollision[11]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_UpperArm");
	m_vecCollision[12]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_UpperArm");
	m_vecCollision[13]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine");
	m_vecCollision[14]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine1");
	m_vecCollision[15]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine2");
	m_vecCollision[16]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine4");
	m_vecCollision[17]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_RH");
	m_vecWeaponCollision[0]->SetCombineMatrix(matCombine);
	m_vecWeaponCollision[1]->SetCombineMatrix(matCombine);
}

bool CPlayer::CollisionCheck(void)
{
	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_U) & 0x80)
	{
		if(m_bCollisionPush[0])
		{
			return false;
		}

		m_bCollisionPush[0] = true;

		if(g_bCollision_Sphere)
		{
			g_bCollision_Sphere = false;
		}
		else
			g_bCollision_Sphere = true;
	}
	else
		m_bCollisionPush[0] = false;

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_O) & 0x80)
	{
		if(m_bCollisionPush[1])
		{
			return false;
		}

		m_bCollisionPush[1] = true;

		if(g_bCollision_OBB)
		{
			g_bCollision_OBB = false;
		}
		else
			g_bCollision_OBB = true;
	}
	else
		m_bCollisionPush[1] = false;

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_I) & 0x80)
	{
		if(m_bCollisionPush[2])
		{
			return false;
		}

		m_bCollisionPush[2] = true;

		if(g_bCollision_BigSphere)
		{
			g_bCollision_BigSphere = false;
		}
		else
			g_bCollision_BigSphere = true;
	}
	else
		m_bCollisionPush[2] = false;

	return true;
}

void CPlayer::SetScytheState(eDarkKnightAniIndex _AniIndex)
{
	if(m_iAniIndex == _AniIndex)
	{
		return;
	}

	m_iAniIndex = _AniIndex;
	m_pMesh->SetAnimation(m_iAniIndex);

	//이거 호출 할때 이동을 해야한다.. 라는 이야기인데....
}

Engine::CTransform* CPlayer::GetInfo(void)
{
	return m_pInfo;
}

void CPlayer::PlayerHit(int iIndex, D3DXVECTOR3 vPos, bool _bStrong)
{
	if(m_eState == Player_Hit || m_eState == Player_Transform || m_eState == Player_ManaAmber)
	{
		return;
	}

	if(m_iAniIndex == PlayerAniIndex_Magic_FireShock || m_iAniIndex == PlayerAniIndex_Magic_FireBolt 
		|| m_iAniIndex == PlayerAniIndex_Magic_FireShock || m_iAniIndex == PlayerAniIndex_Magic_IceBlow 
		|| m_iAniIndex == PlayerAniIndex_Magic_IceSpaer)
	{
		return;
	}

	D3DXVECTOR3 vDir = vPos - m_pInfo->m_vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	m_eState = Player_Hit;

	float fDot = D3DXVec3Dot(&vDir, &m_pInfo->m_vDir);

	if(fDot >= 0.f)
	{
		if(_bStrong)
		{
			if(m_eDarkKnightType == DarkKnight_Scythe)
			{
				m_iAniIndex = DarkKnightAniIndex_BigDamege_Front_Begin;
			}
			else
				m_iAniIndex = PlayerAniIndex_BigDamege_Front;
			
		}
		else
		{
			if(m_eDarkKnightType == DarkKnight_Scythe)
			{
				m_iAniIndex = DarkKnightAniIndex_Damege_Front;
			}
			else
			{
				m_iAniIndex = PlayerAniIndex_Damage_Front;
			}			
		}
	}
	else
	{
		if(_bStrong)
		{
			if(m_eDarkKnightType == DarkKnight_Scythe)
			{
				m_iAniIndex = DarkKnightAniIndex_BigDamege_Back_Begin;
			}
			else
				m_iAniIndex = PlayerAniIndex_BigDamege_Back;
		}
		else
		{
			if(m_eDarkKnightType == DarkKnight_Scythe)
			{
				m_iAniIndex = DarkKnightAniIndex_Damege_Back;
			}
			else
			{
				m_iAniIndex = PlayerAniIndex_Damage_Back;
			}
		}
	}

	CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_hurt11.wav");	
	

	m_pMesh->SetAnimation(m_iAniIndex);
}

bool CPlayer::CollisionSphereCheck(D3DXVECTOR3 _vPos, D3DXVECTOR3 vDir, float fTime, float fSpeed)
{
	if(m_pMonster == NULL)
	{
		m_pMonster = (CKadan*)Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Kadan");
	}

	D3DXVECTOR3 vPos = _vPos;

	vPos += vDir * fTime * fSpeed;

	Engine::CTransform* pInfo = (Engine::CTransform*)m_pMonster->GetComponent(L"Transform");

	D3DXVECTOR3 vDist = vPos - pInfo->m_vPos;
	
	float fDist = D3DXVec3Length(&vDist);
	float fRadius = m_fSlideRadius + 25.f;

	if(fRadius <= fDist)
	{
		return false;
	}
	else
	{
		D3DXVec3Normalize(&m_pInfo->m_vDir, &vDist);
		return true;
	}
}