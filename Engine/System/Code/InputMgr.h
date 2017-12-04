/*!
 * \file InputMgr.h
 * \date 2016/07/08 19:50
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

#ifndef InputMgr_h__
#define InputMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CInputMgr
{
private:
	DECLARE_SINGLETON(CInputMgr);

private:
	LPDIRECTINPUT8			m_pInput;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

private:
	BYTE					m_byKeyState[256];
	DIMOUSESTATE			m_MouseState;
	D3DXVECTOR3				m_vMousePos;
	HWND					m_hWnd;

public:
	HRESULT InitInputDevice(HINSTANCE hInst, HWND hWnd);

private:
	HRESULT InitKeyBoard(HWND hWnd);
	HRESULT InitMouse(HWND hWnd);
	void	Release(void);

public:
	BYTE GetDIKeyState(BYTE KeyFlag);
	BYTE GetDIMouseState(eMouseClick KeyFlag);
	long GetDIMouseMove(eMouseMove KeyFlag);
	D3DXVECTOR3 GetMousePos(void);

public:
	void SetInputState(void);

private:
	CInputMgr(void);
	~CInputMgr(void);
};

END

#endif // InputMgr_h__