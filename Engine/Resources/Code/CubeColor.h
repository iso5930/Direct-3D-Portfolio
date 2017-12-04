/*!
 * \file CubeColor.h
 * \date 2016/08/11 19:28
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

#ifndef CubeColor_h__
#define CubeColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor : public CVIBuffer
{
public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, const DWORD& dwColor);

public:
	virtual HRESULT		CreateBuffer(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, const DWORD& dwColor);
	virtual CResources* CloneResource(void);

private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubeColor(void);
};

END

#endif // CubeColor_h__