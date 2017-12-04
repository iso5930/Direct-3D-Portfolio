/*!
 * \file Magic_UI.h
 * \date 2016/08/07 22:24
 *
 * \author Jin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Magic_UI_h__
#define Magic_UI_h__

#include "GameObject.h"

namespace Engine
{
	class C2DSprite;
}

class CMagic_UI : public Engine::CGameObject
{
private:
	Engine::C2DSprite*	m_p2DSprite;
	Engine::C2DSprite*	m_p2DSpriteICon;
	LPD3DXSPRITE		m_pSprite;
	bool		m_bEnable;
	float		m_fCharge;

public:
	static CMagic_UI* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);

public:
	void SetEnable(bool _Enable);

private:
	CMagic_UI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMagic_UI(void);
};

#endif // Magic_UI_h__