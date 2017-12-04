#ifndef Const_h__
#define Const_h__

const int WINCX = 1900;
const int WINCY = 1060;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

const D3DXVECTOR3 g_vLightPos(0.f, 1500.f, 0.f);
const D3DXVECTOR3 g_vLook(0.f, 0.f, 1.f);

const DWORD KEY_LEFT	= 0x00000001;
const DWORD KEY_RIGHT	= 0x00000002;
const DWORD KEY_UP		= 0x00000004;
const DWORD KEY_DOWN	= 0x00000008;
const DWORD KEY_SPACE	= 0x00000010;

#endif // Const_h__