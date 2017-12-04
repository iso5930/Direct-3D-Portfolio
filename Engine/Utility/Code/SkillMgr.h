/*!
 * \file SkillMgr.h
 * \date 2016/08/13 18:24
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

#ifndef SkillMgr_h__
#define SkillMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CSkillMgr
{
public:
	DECLARE_SINGLETON(CSkillMgr);

private:
	list<CGameObject*> m_SkillList;
	LPDIRECT3DDEVICE9	m_pDevice;
	list<CGameObject*>	m_KadanSkillList;

public:
	void	AddSkill(CGameObject* pSkill);
	void	AddKadanSkill(CGameObject* pSkill);

public:
	list<CGameObject*>* GetList(void);
	list<CGameObject*>* GetKadanSkill(void);

public:
	void	Update(void);
	void	Render(void);
	void	Release(void);

private:
	CSkillMgr(void);
	~CSkillMgr(void);
};

END

#endif // SkillMgr_h__