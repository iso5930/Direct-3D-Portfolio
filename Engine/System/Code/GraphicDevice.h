/*!
 * \file GraphicDevice.h
 * \date 2016/06/29 11:15
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

#ifndef GraphicDevice_h__
#define GraphicDevice_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice
{
public:
	DECLARE_SINGLETON(CGraphicDevice);

public:
	enum WINMODE {MODE_FULL, MODE_WIN};

private:
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice;

public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
	void Release(void);

public:
	LPDIRECT3DDEVICE9 GetDevice(void);

private:
	explicit CGraphicDevice(void);

public:
	~CGraphicDevice(void);
};

END

#endif // GraphicDevice_h__