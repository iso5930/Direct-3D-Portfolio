#include "Skill.h"

Engine::CSkill::CSkill(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_bDeath(false)
{

}

Engine::CSkill::~CSkill(void)
{

}

bool Engine::CSkill::GetDeath(void)
{
	return m_bDeath;
}

void Engine::CSkill::SetDeath(bool _Death)
{
	m_bDeath = _Death;
}

eSkillType Engine::CSkill::GetSkillType(void)
{
	return m_eType;
}