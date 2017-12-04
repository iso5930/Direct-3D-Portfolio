#include "Font.h"

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice), m_pFont(NULL), m_iHeight(0), m_iWidth(0)
{

}

Engine::CFont::~CFont(void)
{
	Release();
}

Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pDevice, UINT iHeight, UINT iWidth, UINT iWeight, TCHAR* pFaceName)
{
	CFont*		pFont = new CFont(pDevice);

	if(FAILED(pFont->Init_Font(iHeight, iWidth, iWeight, pFaceName)))
		Safe_Delete(pFont);

	return pFont;
}

HRESULT Engine::CFont::Init_Font(UINT& iHeight, UINT& iWidth, UINT& iWeight, TCHAR* pFaceName)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.Height = m_iHeight = iHeight;
	Font_Desc.Width = m_iWidth = iWidth;
	Font_Desc.Weight = iWeight;
	lstrcpy(Font_Desc.FaceName, pFaceName);
	Font_Desc.CharSet = HANGEUL_CHARSET;

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CFont::Release(void)
{
	Safe_Release(m_pFont);
}

void Engine::CFont::Render(TCHAR* pString, D3DXCOLOR Color, D3DXVECTOR3& vPos)
{
	RECT	rc;

	rc.left		= long(vPos.x);
	rc.top		= long(vPos.y);
	rc.right	= long(vPos.x) + m_iWidth * lstrlen(pString);
	rc.bottom	= long(vPos.y) + m_iHeight;

	m_pFont->DrawTextW(NULL, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
}