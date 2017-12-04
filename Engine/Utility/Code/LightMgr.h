/*!
 * \file LightMgr.h
 * \date 2016/07/08 19:28
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

#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLight;

class ENGINE_DLL CLightMgr
{
private:
	DECLARE_SINGLETON(CLightMgr);

private:
	list<CLight*> m_Lightlist;

public:
	HRESULT AddLight(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, const WORD& wLightIdx);

private:
	void Release(void);

public:
	const D3DLIGHT9* GetLightInfo(const UINT& iIndex = 0);

private:
	CLightMgr(void);
	~CLightMgr(void);
};
END

#endif // LightMgr_h__