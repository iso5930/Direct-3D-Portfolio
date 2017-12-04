#include "stdafx.h"
#include "CollisionMgr.h"
#include "Collision.h"
#include "Export_Function.h"
#include "skill.h"
#include "FireBolt.h"
#include "IceBlast.h"
#include "MagicArrow.h"
#include "FireShock.h"
#include "IceBlow.h"
#include "Transform.h"
#include "Kadan.h"
#include "DarkHurricane.h"
#include "Player.h"
#include "DarkMagic.h"

IMPLEMENT_SINGLETON(CCollisionMgr);

CCollisionMgr::CCollisionMgr(void)
{

}

CCollisionMgr::~CCollisionMgr(void)
{

}

bool CCollisionMgr::Collision_OBB(Engine::CCollision* pDestOBB, Engine::CCollision* PSourOBB)
{
	m_OBB[0] = *(pDestOBB->GetOBB());
	m_OBB[1] = *(PSourOBB->GetOBB());

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_OBB[0].vPoint[i], &m_OBB[0].vPoint[i], pDestOBB->GetWorldMatrix());
		D3DXVec3TransformCoord(&m_OBB[1].vPoint[i], &m_OBB[1].vPoint[i], PSourOBB->GetWorldMatrix());
	}
	D3DXVec3TransformCoord(&m_OBB[0].vCenter, &m_OBB[0].vCenter, pDestOBB->GetWorldMatrix());
	D3DXVec3TransformCoord(&m_OBB[1].vCenter, &m_OBB[1].vCenter, PSourOBB->GetWorldMatrix());

	InitAxis(&m_OBB[0]);
	InitAxis(&m_OBB[1]);

	float fDist[3];

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			fDist[0] = fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[0], &m_OBB[i].vAxis[j])) + fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[1], &m_OBB[i].vAxis[j])) + fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[2], &m_OBB[i].vAxis[j]));
			fDist[1] = fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[0], &m_OBB[i].vAxis[j])) + fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[1], &m_OBB[i].vAxis[j])) + fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[2], &m_OBB[i].vAxis[j]));

			D3DXVECTOR3 vTemp = m_OBB[1].vCenter - m_OBB[0].vCenter;

			fDist[2] = fabs(D3DXVec3Dot(&vTemp, &m_OBB[i].vAxis[j]));

			if(fDist[2] > fDist[1] + fDist[0])
			{
				return false;
			}
		}
	}
	return true;
}

bool CCollisionMgr::Collision_Sphere(D3DXVECTOR3 vDestPos, float fDestRadius, D3DXVECTOR3 vSourPos, float fSourRadius)
{
	D3DXVECTOR3 vDist = vDestPos - vSourPos;

	float fLength = D3DXVec3Length(&vDist);
	float fRadius = fDestRadius + fSourRadius;

	if(fRadius <= fLength)
	{
		return false;
	}

	return true;
}

bool CCollisionMgr::Collision_OBB_To_Sphere(Engine::CCollision* pDestOBB, D3DXVECTOR3 vSourPos, float fSourRadius)
{
	return true;
}

bool CCollisionMgr::Collision_IceBlow(CIceBlow* pSkill)
{	
	vector<Engine::CCollision*> vecMonster = (*m_pvecCollision[1]);

	
	size_t iSize = vecMonster.size();

	eSkillType eType = pSkill->GetSkillType();


	list<Engine::CCollision*>* CollisionList = pSkill->GetCollisionList();

	list<Engine::CCollision*>::iterator iceIter = CollisionList->begin();
	list<Engine::CCollision*>::iterator iceIter_end = CollisionList->end();

	for(iceIter; iceIter != iceIter_end; ++iceIter)
	{
		for(size_t i = 0; i < iSize; ++i)
		{
			if(Collision_Sphere((*iceIter)->GetPos(), (*iceIter)->GetRadius(), vecMonster[i]->GetPos(), vecMonster[i]->GetRadius()))
			{
				if(Collision_OBB((*iceIter), vecMonster[i]))
				{
					pSkill->SetCollisionCheck(true);

					Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Kadan");

					((CKadan*)pObj)->KadanHit(i, (*iceIter)->GetPos(), true, eType);
					return true;
				}
			}
		}
	}

	return false;
}

bool CCollisionMgr::Collision_IceBlow2(CIceBlow* pSkill)
{
	vector<Engine::CCollision*> vecMonster = (*m_pvecCollision[1]);

	size_t iSize = vecMonster.size();

	eSkillType eType = pSkill->GetSkillType();

	list<Engine::CCollision*>* CollisionList = pSkill->GetCollisionList2();

	list<Engine::CCollision*>::iterator iceIter = CollisionList->begin();
	list<Engine::CCollision*>::iterator iceIter_end = CollisionList->end();

	for(iceIter; iceIter != iceIter_end; ++iceIter)
	{
		for(size_t i = 0; i < iSize; ++i)
		{
			if(Collision_Sphere((*iceIter)->GetPos(), (*iceIter)->GetRadius(), vecMonster[i]->GetPos(), vecMonster[i]->GetRadius()))
			{
				if(Collision_OBB((*iceIter), vecMonster[i]))
				{
					pSkill->SetCollisionCheck2(true);
					//cout << "아이스 블로 OBB 충돌 !!" << endl;

					Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Kadan");
					((CKadan*)pObj)->KadanHit(i, (*iceIter)->GetPos(), true, eType);

					return true;
				}
			}
		}
	}

	return false;
}

int CCollisionMgr::PlayerToKadan(void)
{
	if(Collision_Sphere((*m_pPlayerPos), m_fPlayerRadius, (*m_pKadanPos), m_fKadanRadius))
	{
		vector<Engine::CCollision*> vecPlayer = (*m_pvecWeaponPlayer);
		vector<Engine::CCollision*> vecMonster = (*m_pvecCollision[1]);
		
		size_t iWeaponSize = vecPlayer.size();
		size_t iMonsterSize = vecMonster.size();		

		for(size_t i = 0; i < iWeaponSize; ++i)
		{
			for(size_t j = 0; j < iMonsterSize; ++j)
			{
				if(Collision_Sphere(vecPlayer[i]->GetPos(), vecPlayer[i]->GetRadius(), vecMonster[j]->GetPos(), vecMonster[j]->GetRadius()))
				{
					if(Collision_OBB(vecPlayer[i], vecMonster[j]))
					{
						return j;
					}
				}
			}
		}
	}
	
	return -1;
}

int CCollisionMgr::KadanToPlayer(void)
{
	if(Collision_Sphere((*m_pKadanPos), m_fKadanRadius, (*m_pPlayerPos), m_fPlayerRadius))
	{
		vector<Engine::CCollision*> vecPlayer = (*m_pvecCollision[0]);
		Engine::CCollision* pKadanWepon = m_pWeaponKadan;

		size_t iPlayerSize = vecPlayer.size();

		for(size_t i = 0; i < iPlayerSize; ++i)
		{
			if(Collision_Sphere(vecPlayer[i]->GetPos(), vecPlayer[i]->GetRadius(), pKadanWepon->GetPos(), pKadanWepon->GetRadius()))
			{
				if(Collision_OBB(vecPlayer[i], pKadanWepon))
				{
					return i;
				}
			}
		}
	}

	return -1;
}

bool CCollisionMgr::SkillToMonster(void)
{
	list<Engine::CGameObject*>* pSkillList = Engine::Get_SkillMgr()->GetList();

	list<Engine::CGameObject*>::iterator iter = pSkillList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pSkillList->end();

	vector<Engine::CCollision*> vecMonster = (*m_pvecCollision[1]);

	for(iter; iter != iter_end; ++iter)
	{
		size_t iSize = vecMonster.size();

		eSkillType eType = ((Engine::CSkill*)(*iter))->GetSkillType();
		
		if(eType == Skill_FireShock || eType == Skill_IceBlow)
		{
			if(eType == Skill_FireShock)
			{
				D3DXVECTOR3 vDir = ((CFireShock*)*iter)->GetInfo()->m_vPos - (*m_pKadanPos);
				float fDist = D3DXVec3Length(&vDir);

				if(fDist <= 60.f)
				{
					Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Kadan");

					((CKadan*)pObj)->KadanFireShock();
				}
			}
			else
			{
				//1회만 충돌처리를 해야하니깐....
				CIceBlow* pSkill = (CIceBlow*)(*iter);

				if(pSkill->GetCollisionCheck() == false)
				{
					if(Collision_Sphere(pSkill->GetCollision()->GetPos(), pSkill->GetCollision()->GetRadius(), (*m_pKadanPos), m_fKadanRadius))
					{
						Collision_IceBlow(pSkill);
					}
				}
				else if(pSkill->GetCreate() == true)
				{
					if(pSkill->GetCollisionCheck2() == false)
					{
						Collision_IceBlow2(pSkill);
					}
				}
			}		
		}
		else
		{
			Engine::CCollision* pCollision = NULL;

			switch(eType)
			{
			case Skill_FireBolt:
				pCollision = ((CFireBolt*)(*iter))->GetCollision();
				break;

			case Skill_IceBlast:
				pCollision = ((CIceBlast*)(*iter))->GetCollision();
				break;

			case Skill_MagicArrow:
				pCollision = ((CMagicArrow*)(*iter))->GetCollision();
				break;
			}

			if(Collision_Sphere(pCollision->GetPos(), pCollision->GetRadius(), (*m_pKadanPos), m_fKadanRadius))
			{
				for(size_t i = 0; i < iSize; ++i)
				{
					if(Collision_Sphere(pCollision->GetPos(), pCollision->GetRadius(), vecMonster[i]->GetPos(), vecMonster[i]->GetRadius()))
					{
						if(Collision_OBB(pCollision, vecMonster[i]))
						{
							((Engine::CSkill*)*iter)->SetDeath(true);
							
							Engine::CGameObject* pObj = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Kadan");
							((CKadan*)pObj)->KadanHit(i, pCollision->GetPos(), true, eType);
							break;
						}
					}
				}
			}
		}			
	}
	
	return false;
}	

bool CCollisionMgr::SkillToPlayer(void)
{	
	vector<Engine::CCollision*> vecPlayer = (*m_pvecCollision[0]);
	list<Engine::CGameObject*>* pSkillList = Engine::Get_SkillMgr()->GetKadanSkill();

	list<Engine::CGameObject*>::iterator iter = pSkillList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pSkillList->end();

	for(iter; iter != iter_end; ++iter)
	{
		eSkillType eType = ((Engine::CSkill*)*iter)->GetSkillType();

		if(((Engine::CSkill*)*iter)->GetDeath())
		{
			continue;
		}

		if(eType == Skill_DarkHurricane)
		{
			Engine::CCollision* pCollision = ((CDarkHurricane*)(*iter))->GetCollision();

			D3DXVECTOR3 vDir = pCollision->GetPos() - (*m_pPlayerPos);

			float fDist = D3DXVec3Length(&vDir);

			if(pCollision->GetRadius() >= fDist)
			{
				((Engine::CSkill*)(*iter))->SetDeath(true);
				((CPlayer*)(Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Player")))->PlayerHit(-1, pCollision->GetPos(), true);
			}
		}
		else if(eType == Skill_DarkBolt)
		{
			D3DXVECTOR3 vDir = ((CDarkMagic*)(*iter))->GetInfo()->m_vPos - (*m_pPlayerPos);

			float fDist = D3DXVec3Length(&vDir);

			if(60.f >= fDist)
			{
				//cout << "마법 충돌" << endl;

				((Engine::CSkill*)(*iter))->SetDeath(true);
				((CPlayer*)(Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Player")))->PlayerHit(-1, ((CDarkMagic*)(*iter))->GetInfo()->m_vPos, true);
			}
		}
		else if(eType == Skill_DarkBolt2)
		{
			
		}
		else if(eType == Skill_BlackHole)
		{

		}
	}

	return false;
}

void CCollisionMgr::AddData(int iIndex, vector<Engine::CCollision*>* pvecCollision)
{
	m_pvecCollision[iIndex] = pvecCollision;
}

void CCollisionMgr::InitData(D3DXVECTOR3* vPos, float fRadius, eCollisionType _Type)
{
	switch(_Type)
	{
	case Collision_Player:
		m_pPlayerPos = vPos;
		m_fPlayerRadius = fRadius;
		break;

	case Collision_Kadan:
		m_pKadanPos = vPos;
		m_fKadanRadius = fRadius;
		break;
	}
}

void CCollisionMgr::PlayerWeapon(vector<Engine::CCollision*>* pvecPlayerWeapon)
{
	m_pvecWeaponPlayer = pvecPlayerWeapon;
}

void CCollisionMgr::KadanWeapon(Engine::CCollision* pWeaponCollision)
{
	m_pWeaponKadan = pWeaponCollision;
}

void CCollisionMgr::InitAxis(Engine::OBB* pOBB)
{
	//x
	pOBB->vProjAxis[0] = (pOBB->vPoint[1] + pOBB->vPoint[2] + pOBB->vPoint[5] + pOBB->vPoint[6]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[0] = pOBB->vPoint[1] - pOBB->vPoint[0];

	//y
	pOBB->vProjAxis[1] = (pOBB->vPoint[0] + pOBB->vPoint[1] + pOBB->vPoint[4] + pOBB->vPoint[5]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];

	//z
	pOBB->vProjAxis[2] = (pOBB->vPoint[4] + pOBB->vPoint[5] + pOBB->vPoint[6] + pOBB->vPoint[7]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for(int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pOBB->vAxis[i], &pOBB->vAxis[i]);
}