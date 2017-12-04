#include "stdafx.h"
#include "Include.h"
#include "Kadan.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Map.h"
#include "Collision.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "BlinkEffect.h"
#include "EffectMgr.h"
#include "KadanSpin.h"
#include "DarkHurricane.h"
#include "DarkMagic.h"
#include "SoundMgr.h"
#include "DarkBomb2.h"

CKadan::CKadan(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pInfo(NULL), m_pMesh(NULL), m_eAniIndex(KadanAniIndex_Idie)
, m_pMapMesh(NULL), m_pPlayer(NULL), m_eState(Kadan_Idle), m_fIdleAccTime(0.f), m_bMoveCheck(false)
, m_fAttAccTime(0.f), m_bAttEnd(false), m_bBlink(false), m_fMoveAccTime(0.f), m_bCreate(false)
, m_bFourAttackCheck(false), m_bIce(false), m_fIceAccTime(0.f), m_bRage(false)
{
	
}

CKadan::~CKadan(void)
{
	Release();
}

CKadan* CKadan::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CKadan* pObj = new CKadan(pDevice);

	if(FAILED(pObj->Initialize()))
	{
		Engine::Safe_Delete(pObj);
	}

	return pObj;
}

HRESULT CKadan::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_eAniIndex = KadanAniIndex_Idie;
	m_pMesh->SetAnimation(m_eAniIndex);

	m_fRadius = 170.f;

	HRESULT hr = D3DXCreateSphere(m_pDevice, m_fRadius, 10, 10, &m_pSphere, 0);
	FAILED_CHECK(hr);

	hr = D3DXCreateSphere(m_pDevice, 25.f, 10, 10, &m_pSphere2, 0);
	FAILED_CHECK(hr);

	m_pInfo->m_vPos = D3DXVECTOR3(173.f, 35.f, 3128.f);
	
	const D3DXMATRIX* matInfo = &m_pInfo->m_matWorld;
	const D3DXMATRIX* matCombine = NULL;
	Engine::CCollision* pCollision = NULL;

	matCombine = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_RH");
	pCollision = Engine::CCollision::Create(m_pDevice, 110.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(60.f, 0.f, -3.f));
	pCollision->SetScale(D3DXVECTOR3(110.f, 5.f, 18.f));
	m_pWeaponCollision = pCollision;

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Head");
	pCollision = Engine::CCollision::Create(m_pDevice, 10.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(12.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Thigh");
	pCollision = Engine::CCollision::Create(m_pDevice, 25.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(10.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(25.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Thigh");
	pCollision = Engine::CCollision::Create(m_pDevice, 25.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(10.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(25.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Calf");
	pCollision = Engine::CCollision::Create(m_pDevice, 20.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(12.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(20.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Calf");
	pCollision = Engine::CCollision::Create(m_pDevice, 20.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(12.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(20.f, 10.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Hand");
	pCollision = Engine::CCollision::Create(m_pDevice, 5.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(3.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(5.f, 5.f, 5.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");
	pCollision = Engine::CCollision::Create(m_pDevice, 5.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(3.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(5.f, 5.f, 5.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Pelvis");
	pCollision = Engine::CCollision::Create(m_pDevice, 20.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 20.f, 12.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_ForeTwist");
	pCollision = Engine::CCollision::Create(m_pDevice, 15.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(15.f, 10.f, 7.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_UpperArm");
	pCollision = Engine::CCollision::Create(m_pDevice, 15.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(15.f, 10.f, 7.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_ForeTwist");
	pCollision = Engine::CCollision::Create(m_pDevice, 15.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(15.f, 10.f, 7.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_UpperArm");
	pCollision = Engine::CCollision::Create(m_pDevice, 15.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(5.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(15.f, 10.f, 7.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Shoulder01");
	pCollision = Engine::CCollision::Create(m_pDevice, 11.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(3.f, 5.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(11.f, 11.f, 11.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Shoulder01");
	pCollision = Engine::CCollision::Create(m_pDevice, 15.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(2.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 15.f, 10.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine");
	pCollision = Engine::CCollision::Create(m_pDevice, 17.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 17.f, 14.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine1");
	pCollision = Engine::CCollision::Create(m_pDevice, 17.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 17.f, 14.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine2");
	pCollision = Engine::CCollision::Create(m_pDevice, 17.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 17.f, 17.f));
	m_vecCollision.push_back(pCollision);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine4");
	pCollision = Engine::CCollision::Create(m_pDevice, 17.f, matInfo, matCombine);
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	pCollision->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCollision->SetScale(D3DXVECTOR3(10.f, 17.f, 17.f));
	m_vecCollision.push_back(pCollision);

	CCollisionMgr::GetInstance()->AddData(1, &m_vecCollision);
	CCollisionMgr::GetInstance()->InitData(&m_pInfo->m_vPos, m_fRadius, Collision_Kadan);
	CCollisionMgr::GetInstance()->KadanWeapon(m_pWeaponCollision);

	m_pSpin[0] = CKadanSpin::Create(m_pDevice);
	m_pSpin[1] = CKadanSpin::Create(m_pDevice);
	m_pSpin[1]->SetAngle(D3DXToRadian(360.f));

	return S_OK;
}

int CKadan::Update(void)
{
	if(m_pMapMesh == NULL)
	{
		InitData();
		//Init데이터
	}

	if(m_eState == Kadan_End)
	{
		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_bBlink = true;
			m_pMesh->SetTrackPos(m_pMesh->GetPeriod() - 0.2);
		}

		Engine::CGameObject::Update();

		return 0;
	}

	float		fTime = Engine::Get_TimeMgr()->GetTime();
	D3DXVECTOR3 vDist = m_pPlayer->GetInfo()->m_vPos - m_pInfo->m_vPos;
	float		fDist = D3DXVec3Length(&vDist);

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_N) & 0x80)
	{
		m_eState = Kadan_Transform;
		m_eAniIndex = KadanAniIndex_Transform;
		m_pMesh->SetAnimation(m_eAniIndex);
		m_bMoveCheck = false;
	}

	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_M) & 0x80)
	{
		if(m_bRage)
		{
			m_eState = Kadan_End;
			m_eAniIndex = KadanAniIndex_End;
			m_pMesh->SetAnimation(m_eAniIndex);
		}		
	}

	if(m_bIce)
	{
		m_fIceAccTime += fTime;

		if(m_fIceAccTime >= 3.f)
		{
			m_fIceAccTime = 0.f;
			m_bIce = false;
		}
	}

	if(m_bCreate == false)
	{
		Engine::CGameObject::Update();

		SetAngle(); //Dir방향에 맞게 몬스터가 회전
		DirCheck(m_pInfo->m_vDir, fTime, 10.f, m_pInfo->m_vPos.y); //높이값 체크

		CCollisionUpdate(); //콜리젼 싱크 맞쳐주기

		if(fDist <= 600.f)
		{
			m_bCreate = true;
		}

		return 0;
	}	

	if(m_eState != Kadan_Attack)
	{
		D3DXVec3Normalize(&m_pInfo->m_vDir, &vDist);
	}
	//매시간마다 Dir을 구해준다.

	//카단 패턴 
	//이동, 대쉬(순간이동), 내려베기, 올려베기, 4연베기, 변신, 대기

	if(m_bAttEnd)
	{
		m_fAttAccTime += fTime;

		if(m_fAttAccTime >= 2.f)
		{
			m_fAttAccTime = 0.f;
			m_bAttEnd = false;
		}
	}

	switch(m_eState)
	{
	case Kadan_Idle:
		
		if(fDist <= 200.f || m_bAttEnd == false)
		{
			if(m_bAttEnd == false)
			{
				m_eState = Kadan_Attack;

				int iRand = 0;

				if(m_bRage)
				{
					iRand = 5;
				}
				else
					iRand = 4;
				
				int iPatton = rand() % iRand;

				switch(iPatton)
				{
				case 0:
					m_eAniIndex = KadanAniIndex_TeleportAtt1;
					break;

				case 1:
					m_eAniIndex = KadanAniIndex_TeleportAtt2;
					break;

				case 2:
					m_eAniIndex = KadanAniIndex_UpperAttack;
					break;

				case 3:
					m_eAniIndex = KadanAniIndex_FourAttack;
					break;

				case 4:
					m_eAniIndex = KadanAniIndex_Bolt;
					break;
				}
				m_bCollision = true;

				m_pMesh->SetAnimation(m_eAniIndex);
			}
			else
			{
				m_eState = Kadan_Move;
				m_eAniIndex = KadanAniIndex_Walk;
				m_bMoveCheck = true;
				m_fMoveAccTime = 0.f;
				m_pMesh->SetAnimation(m_eAniIndex);

				m_bBlinkEffectCreate = true;
			}
		}
		else if(fDist >= 500.f)
		{
			m_eState = Kadan_Blink;
			
			m_eAniIndex = KadanAniIndex_Run;
			m_pMesh->SetAnimation(m_eAniIndex);
			m_bMoveCheck = true;
			m_fBlinkTime = 0.f;
			//순간이동을 해야한다.
		}
		else
		{
			m_eState = Kadan_Dash;
			m_eAniIndex = KadanAniIndex_Run;
			m_pMesh->SetAnimation(m_eAniIndex);
			m_bMoveCheck = true;			
			m_fRunAccTime = 0.f;
		}
		break;

	case Kadan_Move:

		m_fMoveAccTime += fTime;

		if(m_fMoveAccTime >= 5.f)
		{
			m_bMoveCheck = false;
			m_fMoveAccTime = 0.f;

			m_eState = Kadan_Idle;
			m_eAniIndex = KadanAniIndex_Idie;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
		break;

	case Kadan_Blink:

		m_fBlinkTime += fTime;
		
		if(m_fBlinkTime >= 1.f)
		{
			m_bBlink = true;
			m_bMoveCheck = false;

			if(m_bBlinkEffectCreate)
			{
				m_bBlinkEffectCreate = false;

				CBlinkEffct* pEffect = CBlinkEffct::Create(m_pDevice);
				pEffect->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 20.f, m_pInfo->m_vPos.z));
				CEffectMgr::GetInstance()->AddEffect(pEffect);
			}
			
			if(m_fBlinkTime <= 4.f)
			{
				m_fBlinkTime = 0.f;
					
				m_eState = Kadan_Attack;
				m_bBlink = false;

				int iPatton = rand() % 2;

				if(iPatton == 0)
				{
					m_eAniIndex = KadanAniIndex_TeleportAtt1;
				}
				else if(iPatton == 1)
				{
					m_eAniIndex = KadanAniIndex_TeleportAtt2;
				}
				
				m_bCollision = true;
				m_pMesh->SetAnimation(m_eAniIndex);

				m_pInfo->m_vPos = m_pPlayer->GetInfo()->m_vPos + (m_pPlayer->GetInfo()->m_vDir * -1) * 50.f;

				CBlinkEffct* pEffect = CBlinkEffct::Create(m_pDevice);
				pEffect->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 10.f, m_pInfo->m_vPos.z));

				CEffectMgr::GetInstance()->AddEffect(pEffect);
			}
		}

		break;

	case Kadan_Attack:
		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_eState = Kadan_Idle;
			m_eAniIndex = KadanAniIndex_Idie;
			m_fAttAccTime = 0.f;
			m_bAttEnd = true;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
		break;

	case Kadan_Hit:
		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_eState = Kadan_Idle;
			m_eAniIndex = KadanAniIndex_Idie;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
		break;

	case Kadan_Transform:
		if(m_pMesh->GetTrackPosition() >= 7.9 && m_bRage == false)
		{
			DOUBLE dTrackPos = m_pMesh->GetTrackPosition();
			m_bRage = true;
			m_pMesh = (Engine::CDynamicMesh*)GetComponent(L"Kadan_Rage");

			Collision_Sync();

			m_pMesh->SetAnimation(m_eAniIndex);
			m_pMesh->SetTrackPos(dTrackPos);
		}

		if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
		{
			m_eState = Kadan_Idle;
			m_eAniIndex = KadanAniIndex_Idie;

			m_pMesh->SetAnimation(m_eAniIndex);
		}
		break;

	case Kadan_Dash:
		m_fRunAccTime += fTime;

		if(fDist <= 75.f || m_fRunAccTime >= 5.f)
		{
			m_bMoveCheck = false;
			m_eState = Kadan_Attack;
			m_fRunAccTime = 0.f;

			int iRandCur = 0;

			if(m_bRage)
			{
				iRandCur = 4;
			}
			else
				iRandCur = 3;
			
			int iRand = rand() % iRandCur;

			switch(iRand)
			{
			case 0:
				m_eAniIndex = KadanAniIndex_FourAttack;
				break;

			case 1:
				m_eAniIndex = KadanAniIndex_TeleportAtt1;
				break;

			case 2:
				m_eAniIndex = KadanAniIndex_TeleportAtt2;
				break;

			case 3:
				m_eAniIndex = KadanAniIndex_UpperAttack;
				break;
			}

			m_bCollision = true;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
		break;
	}

	if(m_eAniIndex == KadanAniIndex_FourAttack)
	{
		DOUBLE dTrackPos = m_pMesh->GetTrackPosition();

		m_bMoveCheck = false;

		if(dTrackPos >= 0.3 && dTrackPos <= 1.0)
		{
			m_bMoveCheck = true;
		}
		else if(dTrackPos >= 2.1 && dTrackPos <= 2.4)
		{
			m_bMoveCheck = true;			
		}
		else if(dTrackPos >= 3.8 && dTrackPos <= 4.7)
		{
			m_bMoveCheck = true;
		}

		ePlayerState eState = m_pPlayer->GetState();

		float fLenght = 0.f;

		if(eState != Player_ManaAmber)
		{
			fLenght = 45.f;
		}
		else
			fLenght = 75.f;

		if(fDist <= fLenght)
		{
			m_bMoveCheck = false;
		}
	}

	if(m_bMoveCheck)
	{
		ePlayerState eState = m_pPlayer->GetState();

		float fSpeed = 0.f;

		if(m_eState == Kadan_Blink || m_eState == Kadan_Dash)
			fSpeed = 200.f;
		else
			fSpeed = 70.f;

		float fLenght = 0.f;

		if(eState != Player_ManaAmber)
		{
			fLenght = 45.f;
		}
		else
			fLenght = 75.f;

		if(fDist >= fLenght)
		{
			if(DirCheck(m_pInfo->m_vDir, fTime, fSpeed, m_pInfo->m_vPos.y))
			{
				CollisionSphereCheck(m_pInfo->m_vPos, m_pInfo->m_vDir, fTime, fSpeed);

				m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * fSpeed;
			}	
		}
		else
		{
			int iRand = rand() % 3;

			if(iRand == 0)
			{
				m_eAniIndex = KadanAniIndex_Idie;
			}
			else
			{
				switch(iRand)
				{
				case 1:
					m_eAniIndex = KadanAniIndex_FourAttack;
					break;

				case 2:
					m_eAniIndex = KadanAniIndex_UpperAttack;
					break;
				}

				m_bCollision = true;
			}
			m_bMoveCheck = false;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
	}


	if(m_eAniIndex == KadanAniIndex_FourAttack)
	{
		if(m_pMesh->GetTrackPosition() >= 1.22 && m_pMesh->GetTrackPosition() <= 1.485) //1타
		{
			if(m_bFourAttackCheck)
			{
				//cout << "1타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;

				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_01.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_01.wav");
			}
		}
		else if(m_pMesh->GetTrackPosition() >= 2.5 && m_pMesh->GetTrackPosition() <= 3.3)//2타
		{
			if(m_bFourAttackCheck)
			{
				//cout << "2타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_02.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_02.wav");
			}			
		}
		else if(m_pMesh->GetTrackPosition() >= 4.1 && m_pMesh->GetTrackPosition() <= 4.5)//3타
		{
			if(m_bFourAttackCheck)
			{
				//cout << "3타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_01.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_01.wav");
			}			
		}
		else if(m_pMesh->GetTrackPosition() >= 5.8 && m_pMesh->GetTrackPosition() <= 6.1)//4타
		{
			if(m_bFourAttackCheck)
			{
				//cout << "4타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_02.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_02.wav");
			}			
		}
		else
		{
			m_bFourAttackCheck = true;
			m_bCollision = false;
		}
	}
	else if(m_eAniIndex == KadanAniIndex_TeleportAtt2)
	{
		if(m_pMesh->GetTrackPosition() >= 1.1 && m_pMesh->GetTrackPosition() <= 1.98)
		{
			if(m_bFourAttackCheck)
			{
				//cout << "텔포 1타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_02.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_02.wav");
			}
		}
		else if(m_pMesh->GetTrackPosition() >= 2.8 && m_pMesh->GetTrackPosition() <= 3.2)
		{
			if(m_bFourAttackCheck)
			{
				//cout << "텔포 2타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_01.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_01.wav");
			}
		}
		else
		{
			m_bFourAttackCheck = true;
			m_bCollision = false;
		}
	}
	else if(m_eAniIndex == KadanAniIndex_TeleportAtt1)
	{
		if(m_pMesh->GetTrackPosition() >= 1.4 && m_pMesh->GetTrackPosition() <= 1.7)
		{
			if(m_bFourAttackCheck)
			{
				//cout << "텔레포트 1타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_02.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_02.wav");
			}
		}
		else
		{
			m_bCollision = false;
			m_bFourAttackCheck = true;
		}
	}
	else if(m_eAniIndex == KadanAniIndex_UpperAttack)
	{
		if(m_pMesh->GetTrackPosition() >= 1.353 && m_pMesh->GetTrackPosition() <= 2.0)
		{
			if(m_bFourAttackCheck)
			{
				//cout << "어퍼컷 1타" << endl;
				m_bCollision = true;
				m_bFourAttackCheck = false;
				
				if(m_bRage)
				{
					CDarkHurricane* pSkill = CDarkHurricane::Create(m_pDevice);
					D3DXVECTOR3 vPos = m_pInfo->m_vPos;
					vPos += m_pInfo->m_vDir * 50.f;
					pSkill->SetPos(vPos);
					pSkill->SetDir(m_pInfo->m_vDir);

					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
				}

				CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_02.wav");
				CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_02.wav");
			}
		}
		else
		{
			m_bFourAttackCheck = true;
			m_bCollision = false;
		}
	}
	else if(m_eAniIndex == KadanAniIndex_Bolt)
	{
		if(m_pMesh->GetTrackPosition() >= 5.6 && m_pMesh->GetTrackPosition() <= 5.8)
		{
			if(m_bFourAttackCheck)
			{
				m_bFourAttackCheck = false;

				if(m_bRage)
				{
					D3DXVECTOR3 vSkillPos = m_pInfo->m_vPos;
					D3DXVECTOR3 vDir = m_pInfo->m_vDir;

					D3DXMATRIX matRotY;
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(45.f));

					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);

					vSkillPos += vDir * 90.f;
					vSkillPos.y += 3.f;
					
					CDarkBomb2* pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CDarkMagic* pSkill = CDarkMagic::Create(m_pDevice);
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 0);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(135.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 90.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 1);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(225.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 90.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 2);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(315.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 90.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 3);

					CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_02.wav");
					CSoundMgr::GetInstance()->PlayKadanSwordSound(L"loop_kadan_attack_02.wav");
				}
			}
		}
		else if(m_pMesh->GetTrackPosition() >= 5.9 && m_pMesh->GetTrackPosition() <= 6.1)
		{
			if(m_bFourAttackCheck)
			{
				m_bFourAttackCheck = false;

				if(m_bRage)
				{
					//cout << "마법 1" << endl;

					D3DXVECTOR3 vSkillPos = m_pInfo->m_vPos;
					D3DXVECTOR3 vDir = m_pInfo->m_vDir;

					D3DXMATRIX matRotY;
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(45.f));

					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);

					vSkillPos += vDir * 180.f;
					vSkillPos.y += 3.f;

					CDarkMagic* pSkill = CDarkMagic::Create(m_pDevice);
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					CDarkBomb2* pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 4);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(135.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 180.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 5);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(225.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 180.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 6);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(315.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 180.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 7);
				}
			}
		}
		else if(m_pMesh->GetTrackPosition() >= 6.2 && m_pMesh->GetTrackPosition() <= 6.4)
		{
			if(m_bFourAttackCheck)
			{
				m_bFourAttackCheck = false;

				if(m_bRage)
				{
					//cout << "마법 2" << endl;

					D3DXVECTOR3 vSkillPos = m_pInfo->m_vPos;
					D3DXVECTOR3 vDir = m_pInfo->m_vDir;

					D3DXMATRIX matRotY;
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(45.f));

					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);

					vSkillPos += vDir * 270.f;
					vSkillPos.y += 3.f;

					CDarkMagic* pSkill = CDarkMagic::Create(m_pDevice);
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					CDarkBomb2* pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 8);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(135.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 270.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 9);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(225.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 270.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 10);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(315.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 270.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 11);
				}
			}
		}
		else if(m_pMesh->GetTrackPosition() >= 6.5 && m_pMesh->GetTrackPosition() <= 6.7)
		{
			if(m_bFourAttackCheck)
			{
				m_bFourAttackCheck = false;

				if(m_bRage)
				{
					//cout << "마법 3" << endl;

					D3DXVECTOR3 vSkillPos = m_pInfo->m_vPos;
					D3DXVECTOR3 vDir = m_pInfo->m_vDir;

					D3DXMATRIX matRotY;
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(45.f));

					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);

					vSkillPos += vDir * 360.f;
					vSkillPos.y += 3.f;

					CDarkMagic* pSkill = CDarkMagic::Create(m_pDevice);
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					CDarkBomb2* pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 12);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(135.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 360.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 13);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(225.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 360.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 14);

					pSkill = CDarkMagic::Create(m_pDevice);
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(315.f));
					D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
					vSkillPos = m_pInfo->m_vPos + (vDir * 360.f);
					vSkillPos.y += 3.f;
					pSkill->GetInfo()->m_vPos = vSkillPos;
					Engine::Get_SkillMgr()->AddKadanSkill(pSkill);
					pEffect = CDarkBomb2::Create(m_pDevice);
					pEffect->SetInfo(vSkillPos);
					CEffectMgr::GetInstance()->AddEffect(pEffect);
					CSoundMgr::GetInstance()->PlayDarkFireSound(L"magic_2.wav", 15);
				}
			}
		}
		else
			m_bFourAttackCheck = true;
	}

	Engine::CGameObject::Update();

	SetAngle(); //Dir방향에 맞게 몬스터가 회전
	DirCheck(m_pInfo->m_vDir, fTime, 10.f, m_pInfo->m_vPos.y); //높이값 체크

	CCollisionUpdate(); //콜리젼 싱크 맞쳐주기

	m_pSpin[0]->SetInfo(m_pInfo->m_vPos);
	m_pSpin[0]->Update();
	m_pSpin[1]->SetInfo(m_pInfo->m_vPos);
	m_pSpin[1]->Update();

	if(m_bCollision)
	{
		int iIndex = CCollisionMgr::GetInstance()->KadanToPlayer();

		if(iIndex != -1)
		{
			m_bCollision = false;
			m_pPlayer->PlayerHit(iIndex, m_pInfo->m_vPos, false);

			//cout << "충돌" << endl;
		}
	}

	return 0;
}

void CKadan::Render(void)
{
	if(m_bBlink == false)
	{
		HRESULT hr = SetConstantTable();
		FAILED_CHECK_RETURN(hr, );
		LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
		NULL_CHECK_RETURN(pEffect, );

		//Engine::Get_ShadowMgr()->ShadowSetConstantTable(&m_pInfo->m_matWorld, g_vLightPos);

		//m_pMesh->Render_Shadow(Engine::Get_ShadowMgr()->GetEffect());

		//Engine::Get_ShadowMgr()->ShadowEnd();

		float fTime = Engine::Get_TimeMgr()->GetTime() * 1.5f;

		if(m_bIce == false)
		{
			m_pMesh->FrameMove(fTime);
		}
		m_pMesh->RenderMesh(pEffect, NULL);

		if(m_bRage)
		{
			m_pSpin[0]->Render();
			m_pSpin[1]->Render();
		}
	}
	
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

		m_pWeaponCollision->SetBOOL(g_bCollision_OBB, g_bCollision_Sphere);
		m_pWeaponCollision->Render();

		if(g_bCollision_BigSphere)
		{
			D3DXVECTOR3 vPos = m_pInfo->m_vPos;
			vPos.y += 35.f;

			D3DXMATRIX matTrans;
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

			m_pDevice->SetTransform(D3DTS_WORLD, &matTrans);
			m_pSphere->DrawSubset(0);
		}
			
		D3DXVECTOR3 vPos = m_pInfo->m_vPos;
		vPos.y += 35.f;

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

		m_pDevice->SetTransform(D3DTS_WORLD, &matTrans);
		m_pSphere2->DrawSubset(0);

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

HRESULT CKadan::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Kadan");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_pMesh->SetOwner(Collision_Kadan);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Mesh", pComponent));

	//Mesh_Rage
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Kadan_Rage");
	((Engine::CDynamicMesh*)pComponent)->SetOwner(Collision_Kadan);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Kadan_Rage", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Kadan");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CKadan::SetConstantTable(void)
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

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVECTOR3 vCameraPos(matView._41, matView._42, matView._43);
	pEffect->SetVector("g_vCameraPos", &D3DXVECTOR4(vCameraPos, 0.f));
	
	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffus", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	return S_OK;
}

void CKadan::Release(void)
{
	size_t iSize = m_vecCollision.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		Engine::Safe_Delete(m_vecCollision[i]);
	}
	m_vecCollision.clear();

	Engine::Safe_Release(m_pSphere);
	Engine::Safe_Release(m_pSphere2);

	Engine::Safe_Delete(m_pWeaponCollision);

	for(int i = 0; i < 2; ++i)
	{
		Engine::Safe_Delete(m_pSpin[i]);
	}
}

bool CKadan::DirCheck(D3DXVECTOR3 vDir, float fTime, float fSpeed, float& fY)
{
	DWORD dwIndex, dwCnt;
	float fU, fV, fDist;

	D3DXVECTOR3 vPos = m_pInfo->m_vPos;
	vPos += vDir * fTime * fSpeed;

	D3DXVECTOR3 vRayDir(0.f, -1.f, 0.f);
	D3DXVECTOR3 vRayPos(vPos.x, vPos.y + 40.f, vPos.z);

	BOOL bCheck  = FALSE;

	bool bHeight = false;
	bool bWall = false;

	SetAngle();

	LPD3DXBUFFER pSubSet = m_pMapMesh->GetSubSet();

	D3DXIntersect(m_pMapMesh->GetMesh(), &vRayPos, &vRayDir, &bCheck, &dwIndex, &fU, &fV, &fDist, &pSubSet, &dwCnt);
	
	if(bCheck == TRUE)
	{
		if(fDist <= 80.f)
		{
			D3DXVECTOR3 vPoint = vRayPos + (vRayDir * fDist);
			
			fY = vPoint.y;

			bHeight = true;
		}
		else
			bHeight = false;
	}
	else
		bHeight = false;

	vRayDir = m_pInfo->m_vDir;

	vRayPos.x = m_pInfo->m_vPos.x;
	vRayPos.y = m_pInfo->m_vPos.y + 15.f;
	vRayPos.z = m_pInfo->m_vPos.z;

	D3DXIntersect(m_pMapMesh->GetMesh(), &vRayPos, &vRayDir, &bCheck, &dwIndex, &fU, &fV, &fDist, &pSubSet, &dwCnt);

	if(bCheck == TRUE)
	{
		if(fDist <= 15.f)
		{
			bWall = true;
		}
		else
			bWall = false;
	}	
	else
		bWall = false;
	
	if((!bWall) && bHeight)
	{
		return true;
	}
	
	return false;
}	

void CKadan::KadanHit(int iIndex, D3DXVECTOR3 vPlayerPos, bool bSkill /*= false*/, eSkillType _Type /*= Skill_End*/)
{
	if(m_eState == Kadan_End)
	{
		return;
	}

	if(bSkill == true)
	{
		if(_Type == Skill_IceBlast)
		{
			m_bIce = true;
		}
	}

	if(m_eState != Kadan_Attack && m_eState != Kadan_Hit)
	{
		m_eState = Kadan_Hit;
		m_bMoveCheck = false;

		D3DXVECTOR3 vDir = vPlayerPos - m_pInfo->m_vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float fDot = D3DXVec3Dot(&vDir, &m_pInfo->m_vDir);

		if(fDot >= 0.f)
		{
			m_eAniIndex = KadanAniIndex_Hit_Bottom;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
		else
		{
			m_eAniIndex = KadanAniIndex_Hit_Back;
			m_pMesh->SetAnimation(m_eAniIndex);
		}
	}

	CSoundMgr::GetInstance()->PlayKadanSound(L"loop_kadan1_03.wav");
}

void CKadan::KadanAttack(eKadanAniIndex _Index)
{
	
}

void CKadan::SetAngle(void)
{
	float fAngle = D3DXVec3Dot(&m_pInfo->m_vDir, &g_vLook);
	fAngle = acosf(fAngle);

	float fAngleX = D3DXVec3Dot(&m_pInfo->m_vDir, &D3DXVECTOR3(1.f, 0.f, 0.f));
	fAngleX = acosf(fAngleX);

	if(D3DXToDegree(fAngleX) >= 90.f)
	{
		fAngle = 2 * D3DX_PI - fAngle;
	}

	m_pInfo->m_fAngle[Angle_Y] = fAngle + D3DXToRadian(90.f);
}

void CKadan::InitData(void)
{
	if(m_pMapMesh == NULL)
	{
		Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_Environment, L"Map");

		if(pObj != NULL)
		{
			m_pMapMesh = ((CMap*)pObj)->GetMesh();
		}
	}

	if(m_pPlayer == NULL)
	{
		Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Player");

		if(pObj != NULL)
		{
			m_pPlayer = ((CPlayer*)pObj);
		}
	}
}	

void CKadan::CCollisionUpdate(void)
{
	size_t iSize = m_vecCollision.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		m_vecCollision[i]->Update();
	}

	m_pWeaponCollision->Update();
}

bool CKadan::CollisionSphereCheck(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir, float _fTime, float _fSpeed)
{
	D3DXVECTOR3 vPos = _vPos;

	vPos += _vDir * _fTime * _fSpeed;

	D3DXVECTOR3 vDir = vPos - m_pPlayer->GetInfo()->m_vPos;

	float fDist = D3DXVec3Length(&vDir);

	float fRadius = 25.f + 15.f;

	if(m_pPlayer->GetState() == Player_ManaAmber)
	{
		fRadius = 25.f + 50.f;
	}	

	if(fRadius <= fDist)
	{
		return false;
	}
	else
	{
		D3DXVec3Normalize(&m_pInfo->m_vDir, &vDir);
		return true;
	}
}

void CKadan::KadanFireShock(void)
{
	//충돌 검사를 0.3 초 마다 
}

void CKadan::Collision_Sync(void)
{
	const D3DXMATRIX* matCombine = NULL;

	matCombine = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_RH");
	m_pWeaponCollision->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Head");
	m_vecCollision[0]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Thigh");
	m_vecCollision[1]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Thigh");
	m_vecCollision[2]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Calf");
	m_vecCollision[3]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Calf");
	m_vecCollision[4]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Hand");
	m_vecCollision[5]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");
	m_vecCollision[6]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Pelvis");
	m_vecCollision[7]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_ForeTwist");
	m_vecCollision[8]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_UpperArm");
	m_vecCollision[9]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_ForeTwist");
	m_vecCollision[10]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_UpperArm");
	m_vecCollision[11]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_R_Shoulder01");
	m_vecCollision[12]->SetCombineMatrix(matCombine);

	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_L_Shoulder01");
	m_vecCollision[13]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine");
	m_vecCollision[14]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine1");
	m_vecCollision[15]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine2");
	m_vecCollision[16]->SetCombineMatrix(matCombine);
	
	matCombine = m_pMesh->FindFrame("ValveBiped_Bip01_Spine4");
	m_vecCollision[17]->SetCombineMatrix(matCombine);
}
