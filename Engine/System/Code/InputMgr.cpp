#include "InputMgr.h"

IMPLEMENT_SINGLETON(Engine::CInputMgr)

Engine::CInputMgr::CInputMgr(void)
:m_pInput(NULL), m_pKeyBoard(NULL), m_pMouse(NULL), m_hWnd(NULL)
{
	ZeroMemory(m_byKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInputMgr::~CInputMgr(void)
{
	Release();
}

HRESULT Engine::CInputMgr::InitInputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = NULL;

	m_hWnd = hWnd;

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK_MSG(hr, L"입력장치 생성 실패");

	hr = InitKeyBoard(hWnd);
	FAILED_CHECK_MSG(hr, L"키보드 생성 실패");

	hr = InitMouse(hWnd);
	FAILED_CHECK_MSG(hr, L"마우스 생성 실패");
	return S_OK;
}

HRESULT Engine::CInputMgr::InitKeyBoard(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	FAILED_CHECK(hr);

	/*m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);*/
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	FAILED_CHECK(hr);

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	FAILED_CHECK(hr);

	m_pKeyBoard->Acquire();

	return S_OK;
}

HRESULT Engine::CInputMgr::InitMouse(HWND hWnd)
{	
	HRESULT		hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	FAILED_CHECK(hr);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	FAILED_CHECK(hr);

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	FAILED_CHECK(hr);

	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInputMgr::Release(void)
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInput);
}

BYTE Engine::CInputMgr::GetDIKeyState(BYTE KeyFlag)
{
	return m_byKeyState[KeyFlag];
}

BYTE Engine::CInputMgr::GetDIMouseState(eMouseClick KeyFlag)
{
	return m_MouseState.rgbButtons[KeyFlag];
}

long Engine::CInputMgr::GetDIMouseMove(eMouseMove KeyFlag)
{
	return *(((long*)&m_MouseState) + KeyFlag);
}

void Engine::CInputMgr::SetInputState(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	POINT ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	m_vMousePos = D3DXVECTOR3((float)ptMouse.x, (float)ptMouse.y, 0.f);
}

D3DXVECTOR3 Engine::CInputMgr::GetMousePos(void)
{
	return m_vMousePos;
}