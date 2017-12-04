/*!
 * \file FontMgr.h
 * \date 2016/07/08 11:22
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

#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CFont;

class ENGINE_DLL CFontMgr
{
private:
	DECLARE_SINGLETON(CFontMgr)

private:
	map<const TCHAR*, CFont*> m_mapFont;

public:
	HRESULT AddFont(LPDIRECT3DDEVICE9 pDevice, TCHAR* pFontKey, UINT iHeight, UINT iWidth, UINT iWeight);
	void	Render(const TCHAR* pFontKey, D3DXVECTOR3 vPos, TCHAR* pString, D3DCOLOR Color);

private:
	void	Release(void);

public:
	CFont*	GetFont(const TCHAR* pFontKey);

private:
	CFontMgr(void);
	~CFontMgr(void);
};

END

#endif // FontMgr_h__