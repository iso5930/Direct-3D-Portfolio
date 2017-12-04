/*!
 * \file Light.h
 * \date 2016/07/08 19:22
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

#ifndef Light_h__
#define Light_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLight
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	D3DLIGHT9			m_LightInfo;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, const WORD& wLightIdx);

public:
	HRESULT Init_Light(const D3DLIGHT9* pLightInfo, const WORD& wLightIdx);

public:
	const D3DLIGHT9* GetLightInfo(void); 

private:
	explicit CLight(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLight(void);
};

END

#endif // Light_h__