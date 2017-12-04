#include "stdafx.h"
#include "Include.h"
#include "IceBlow.h"
#include "Export_Function.h"
#include "Transform.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "Collision.h"
#include "SoundMgr.h"

CIceBlow::CIceBlow(LPDIRECT3DDEVICE9 pDevice)
:CSkill(pDevice), m_pInfo(NULL), m_pShader(NULL), m_pMesh(NULL), m_iAniIndex(0), m_bCreate(false), m_pCollision(NULL)
, m_bCollisionCheck(false), m_bCollisionCheck2(false)
{

}

CIceBlow::~CIceBlow(void)
{
	Release();
}

CIceBlow* CIceBlow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CIceBlow* pSkill = new CIceBlow(pDevice);

	if(FAILED(pSkill->Initialize()))
	{
		Engine::Safe_Delete(pSkill);
	}

	return pSkill;
}

HRESULT CIceBlow::Initialize(void)
{	
	HRESULT hr = AddComponent();	
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pMesh->SetAnimation(0);
	m_pMesh->SetTrackPos(0.0);

	m_eType = Skill_IceBlow;

	const D3DXMATRIX* matCombine = NULL;//m_pMesh->FindFrame("Object078_Bone");
	Engine::CCollision* pCollision = NULL;

	float fSize = 15.f;

	char szFrameName[256] = "Object0%d_Bone";
	char szFullName[256] = "";
	
	for(int i = 0; i < 46; ++i)
	{
		sprintf_s(szFullName, szFrameName, i + 32);
		matCombine = m_pMesh->FindFrame(szFullName);
		pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
		pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
		m_pCollisionList.push_back(pCollision);
	}

	matCombine = m_pMesh->FindFrame("Object060_Bone");
	m_pCollision = Engine::CCollision::Create(m_pDevice, 150.f, &m_pInfo->m_matWorld, matCombine);

	//============== 1Â÷ Ãæµ¹

	Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)GetComponent(L"IceBlow_End");
	matCombine = pMesh->FindFrame("Object032_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object033_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object034_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object036_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object037_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object042_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object043_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object045_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object047_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object048_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object050_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object052_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object053_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object057_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object058_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object059_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object061_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object062_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object063_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object066_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object067_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object073_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object074_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object075_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object077_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object078_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object055_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object055_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object064_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object064_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object040_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object049_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object049_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object041_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object041_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object044_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object054_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object054_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object054_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object070_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object070_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object065_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object065_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object038_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object038_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object038_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object039_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object039_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object051_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object051_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object046_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object046_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object046_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object056_frag_001_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object060_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object060_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object060_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object072_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object035_frag_001_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object041_frag_005_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object041_frag_005_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object041_frag_005_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object040_frag_003_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object071_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object071_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object072_frag_002_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object072_frag_002_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object072_frag_002_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object076_frag_001_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object076_frag_001_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object076_frag_001_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object044_frag_002_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object044_frag_002_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object044_frag_002_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object035_frag_001_frag_003_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object035_frag_001_frag_003_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object035_frag_001_frag_003_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object040_frag_003_frag_003_frag_002_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object040_frag_003_frag_003_frag_003_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object071_frag_003_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object071_frag_003_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object071_frag_003_frag_006_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object071_frag_003_frag_007_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object068_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object068_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	matCombine = pMesh->FindFrame("Object068_frag_006_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object068_frag_007_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object069_frag_004_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object069_frag_005_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object069_frag_006_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);
	matCombine = pMesh->FindFrame("Object069_frag_007_Bone");
	pCollision = Engine::CCollision::Create(m_pDevice, fSize, &m_pInfo->m_matWorld, matCombine);
	pCollision->SetScale(D3DXVECTOR3(20.f, 20.f, 20.f));
	m_pCollisionList2.push_back(pCollision);

	
	CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_ice_spear.wav");
	CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack2.wav");

	
	
	return S_OK;
}	

int CIceBlow::Update(void)
{
	CGameObject::Update();

	if(m_pMesh->GetPeriod() - 0.2f < m_pMesh->GetTrackPosition())
	{
		if(m_bCreate)
		{
			m_pMesh->SetTrackPos(m_pMesh->GetPeriod() - 0.2);
			return 1;
		}
		else
		{
			m_pMesh = (Engine::CDynamicMesh*)GetComponent(L"IceBlow_End");
			m_bCreate = true;

			CSoundMgr::GetInstance()->PlayPlayerSound(L"magic_evy_ice_blast.wav");	
			CSoundMgr::GetInstance()->PlayPlayerAttackSound(L"evy_strongattack3.wav");

			m_pMesh->SetAnimation(0);
			m_pMesh->SetTrackPos(0.0);
		}
	}

	if(m_bCreate == false)
	{
		list<Engine::CCollision*>::iterator iter = m_pCollisionList.begin();
		list<Engine::CCollision*>::iterator iter_end = m_pCollisionList.end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Update();
		}

		m_pCollision->Update();
	}
	else
	{
		list<Engine::CCollision*>::iterator iter = m_pCollisionList2.begin();
		list<Engine::CCollision*>::iterator iter_end = m_pCollisionList2.end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Update();
		}
	}

	

	return 0;
}

void CIceBlow::Render(void)
{
	HRESULT hr = SetConstantTable();
	FAILED_CHECK_RETURN(hr, );

	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK_RETURN(pEffect, );

	float fTime = Engine::Get_TimeMgr()->GetTime() * 2.5f;

	m_pMesh->FrameMove(fTime);
	m_pMesh->RenderMesh(pEffect, NULL);

// 	if(m_bCreate)
// 	{
// 		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 		list<Engine::CCollision*>::iterator iter = m_pCollisionList2.begin();
// 		list<Engine::CCollision*>::iterator iter_end = m_pCollisionList2.end();
// 
// 		for(iter; iter != iter_end; ++iter)
// 		{
// 			(*iter)->SetBOOL(true, true);
// 			(*iter)->Render();
// 		}
// 
// 		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
// 	}	
}

HRESULT CIceBlow::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"IceBlow_Begin");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_pMesh->SetOwner(Collision_Skill);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"IceBlow_Begin", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"IceBlow_End");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	dynamic_cast<Engine::CDynamicMesh*>(pComponent)->SetOwner(Collision_Skill);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"IceBlow_End", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Skill");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"Shader", pComponent));

	return S_OK;
}

void CIceBlow::SetInfo(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CIceBlow::SetAngel(eAngle _Angel, float fAngle)
{
	m_pInfo->m_fAngle[Angle_Y] = fAngle;
}

HRESULT CIceBlow::SetConstantTable(void)
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

void CIceBlow::Release(void)
{
	list<Engine::CCollision*>::iterator iter = m_pCollisionList.begin();
	list<Engine::CCollision*>::iterator iter_end = m_pCollisionList.end();

	for(iter; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete((*iter));
	}
	m_pCollisionList.clear();

	iter = m_pCollisionList2.begin();
	iter_end = m_pCollisionList2.end();

	for(iter; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete((*iter));
	}
	m_pCollisionList2.clear();
}

bool CIceBlow::GetCreate(void)
{
	return m_bCreate;
}

void CIceBlow::SetCollisionCheck(bool _Check)
{
	m_bCollisionCheck = _Check;
}

bool CIceBlow::GetCollisionCheck(void)
{
	return m_bCollisionCheck;
}

void CIceBlow::SetCollisionCheck2(bool _Check)
{
	m_bCollisionCheck2 = _Check;
}

bool CIceBlow::GetCollisionCheck2(void)
{
	return m_bCollisionCheck2;
}

list<Engine::CCollision*>* CIceBlow::GetCollisionList(void)
{
	return &m_pCollisionList;
}

Engine::CCollision* CIceBlow::GetCollision(void)
{
	return m_pCollision;
}

list<Engine::CCollision*>* CIceBlow::GetCollisionList2(void)
{
	return &m_pCollisionList2;
}