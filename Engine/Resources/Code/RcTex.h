/*!
 * \file RcTex.h
 * \date 2016/07/07 20:33
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

#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual CResources* CloneResource(void);
	virtual HRESULT		CreateBuffer(void);

private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRcTex(void);
};

END

#endif // RcTex_h__