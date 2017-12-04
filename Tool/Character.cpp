#include "stdafx.h"
#include "Include.h"
#include "Character.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Export_Function.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pMesh2(NULL), m_pMesh(NULL), m_pInfo(NULL)
{
	
}

CCharacter::~CCharacter(void)
{
	m_pMesh2->Release();
	m_pMesh->Release();
	Safe_Delete(m_pMesh);
	Safe_Delete(m_pInfo);
}

HRESULT CCharacter::Initialize(void)
{
	//m_pMesh = Engine::CDynamicMesh::Create(m_pDevice, L"../Client/bin/Resource/Mesh/Kadan/", L"Kadan.X");
	//m_pMesh = Engine::CDynamicMesh::Create(m_pDevice, L"../Client/bin/Resource/Mesh/Player/", L"Player.X");
	//m_pMesh = Engine::CDynamicMesh::Create(m_pDevice, L"../Client/bin/Resource/Mesh/DarkKnight_Scythe/", L"DarkKnight_Scythe.X");
	m_pMesh = Engine::CDynamicMesh::Create(m_pDevice, L"../Client/bin/Resource/Mesh/Skill/IceBlow/", L"Ice_Blow_Begin.X");
	m_pInfo = Engine::CTransform::Create(D3DXVECTOR3(0.f, 0.f, 1.f));

	m_pMesh2 = ((Engine::CDynamicMesh*)m_pMesh->CloneResource());

	m_pMesh2->SetAnimation(0);
	m_pMesh2->SetTrackPos(0.0);
	
	return S_OK;
}

int CCharacter::Update(void)
{
	m_pInfo->Update();

	m_pMesh2->SetTrackPos(m_pMesh2->GetPeriod() - 0.2);
	
	return 0;
}

void CCharacter::Render(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime() * 1.5f;

	m_pMesh2->FrameMove(fTime);
	m_pMesh2->RenderMesh(NULL, &m_pInfo->m_matWorld);
}