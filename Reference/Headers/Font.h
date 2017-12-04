/*!
 * \file Font.h
 * \date 2016/07/08 11:14
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

#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CFont
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXFONT			m_pFont;
	UINT				m_iHeight;
	UINT				m_iWidth;

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice, UINT iHeight, UINT iWidth, UINT iWeight, TCHAR* pFaceName);

private:
	HRESULT Init_Font(UINT& iHeight, UINT& iWidth, UINT& iWeight, TCHAR* pFaceName);
	void	Release(void);

public:
	void	Render(TCHAR* pString, D3DXCOLOR Color, D3DXVECTOR3& vPos);

private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice);

public:
	~CFont(void);
};

END

#endif // Font_h__