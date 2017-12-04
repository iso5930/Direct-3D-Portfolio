/*!
 * \file Skill.h
 * \date 2016/08/20 20:22
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Skill_h__
#define Skill_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CSkill : public Engine::CGameObject
{
protected:
	bool		m_bDeath;
	eSkillType	m_eType;

public:
	void		SetDeath(bool _Death);
	bool		GetDeath(void);
	eSkillType	GetSkillType(void);

protected:
	explicit CSkill(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkill(void);
};

END

#endif // Skill_h__