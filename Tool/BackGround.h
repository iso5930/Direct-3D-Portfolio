/*!
 * \file BackGround.h
 * \date 2016/07/27 18:00
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

#ifndef BackGround_h__
#define BackGround_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CBackGround : public Engine::CGameObject
{
public:
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	bool					m_bFrame;

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

public:
	void KeyCheck(void);

public:
	CBackGround(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBackGround(void);
};

#endif // BackGround_h__