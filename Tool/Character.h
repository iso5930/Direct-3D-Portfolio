/*!
 * \file Character.h
 * \date 2016/08/11 21:10
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
#ifndef Character_h__
#define Character_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CCharacter : public Engine::CGameObject
{
public:
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CDynamicMesh*	m_pMesh2;
	Engine::CTransform*		m_pInfo;

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

public:
	explicit CCharacter(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCharacter(void);
};

#endif // Character_h__