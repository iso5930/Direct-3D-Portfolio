/*!
 * \file CubeTex.h
 * \date 2016/07/16 22:38
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

#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual CResources* CloneResource(void);
	virtual HRESULT		CreateBuffer(void);
	
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubeTex(void);
};

END

#endif // CubeTex_h__