/*!
 * \file Texture.h
 * \date 2016/07/07 20:39
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

#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
private:
	vector<IDirect3DBaseTexture9*>	m_vecTexture;
	DWORD							m_dwVectorSize;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice, eTexture_Type TextureType, const TCHAR* pFilePath, const WORD& wCnt);
	
public:
	HRESULT LoadTexture(eTexture_Type TextureType, const TCHAR* pFilePath, const WORD& wCnt);
	void	Render(const DWORD& dwStage, const DWORD& iIndex);

public:
	virtual CResources* CloneResource(void);
	virtual DWORD		Release(void);

public:
	void Set_ConstantTable(LPD3DXEFFECT pEffect, const char* pConstantName, const DWORD& dwCnt = 0);

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& rhs);

public:
	virtual ~CTexture(void);
};

END

#endif // Texture_h__