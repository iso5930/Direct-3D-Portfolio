/*!
 * \file CustomColorUI.h
 * \date 2016/08/13 3:05
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

#ifndef CustomColorUI_h__
#define CustomColorUI_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CCustomColor_UI : public Engine::CGameObject
{
private:
	Engine::CTransform* m_pInfo;
	Engine::CVIBuffer*	m_pBuffer;
	Engine::CTexture*	m_pTexture;

private:
	float		m_fX, m_fY;
	float		m_fSizeX, m_fSizeY;
	D3DXMATRIX	m_matView, m_matProj;

private:
	RECT		m_rcCustomColor[3];
	RECT		m_rcCustomParts[4];
	RECT		m_rcColor;
	RECT		m_rcButton[2];

private:
	int			m_iPartIndex;
	int			m_iColorIndex;
	D3DXVECTOR4 m_vCustomColor;

private:
	HDC			m_hDc;
	bool		m_bPush;

public:
	static CCustomColor_UI* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);
	void	DWORD_To_RGBA(DWORD color, D3DXVECTOR4& vColor);
	void	KeyCheck(void);
	
private:
	explicit CCustomColor_UI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCustomColor_UI(void);
};

#endif // CustomColorUI_h__