/*!
 * \file 2DSprite.h
 * \date 2016/08/15 15:57
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

#ifndef C2DSprite_h__
#define C2DSprite_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL C2DSprite : public CResources
{
private:
	vector<TEXINFO*>	m_vecTexture;
	DWORD				m_dwVectorSize;

public:
	static C2DSprite* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, WORD wCnt);

public:
	HRESULT		LoadTexture(const TCHAR* pFilePath, WORD wCnt);
	void		Render(WORD wIndex = 0);
	TEXINFO*	GetTexture(WORD wCnt);

public:
	virtual CResources* CloneResource(void);
	virtual DWORD		Release(void);

private:
	explicit C2DSprite(LPDIRECT3DDEVICE9 pDevice);
	explicit C2DSprite(const C2DSprite& rhs);

public:
	virtual ~C2DSprite(void);
};

END

#endif // C2DSprite_h__