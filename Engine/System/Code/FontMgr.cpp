#include "FontMgr.h"
#include "Font.h"

IMPLEMENT_SINGLETON(Engine::CFontMgr);

Engine::CFontMgr::CFontMgr(void)
{

}

Engine::CFontMgr::~CFontMgr(void)
{
	Release();
}

HRESULT Engine::CFontMgr::AddFont(LPDIRECT3DDEVICE9 pDevice, TCHAR* pFontKey, UINT iHeight, UINT iWidth, UINT iWeight)
{
	map<const TCHAR*, CFont*>::iterator iter = m_mapFont.begin();
	map<const TCHAR*, CFont*>::iterator iter_end = m_mapFont.end();

	map<const TCHAR*, CFont*>::iterator Find_iter = find_if(iter, iter_end, CTagFinder(pFontKey));

	if(Find_iter != iter_end)
	{
		return E_FAIL;
	}

	CFont* pFont = CFont::Create(pDevice, iHeight, iWidth, iWeight, pFontKey);
	
	if(pFont == NULL)
	{
		return E_FAIL;
	}

	m_mapFont.insert(map<const TCHAR*, CFont*>::value_type(pFontKey, pFont));

	return S_OK;
}

void Engine::CFontMgr::Render(const TCHAR* pFontKey, D3DXVECTOR3 vPos, TCHAR* pString, D3DCOLOR Color)
{
	map<const TCHAR*, CFont*>::iterator iter = m_mapFont.begin();
	map<const TCHAR*, CFont*>::iterator iter_end = m_mapFont.end();

	map<const TCHAR*, CFont*>::iterator Find_iter = find_if(iter, iter_end, CTagFinder(pFontKey));

	if(Find_iter == iter_end)
	{
		return;
	}

	Find_iter->second->Render(pString, Color, vPos);
}

void Engine::CFontMgr::Release(void)
{
	map<const TCHAR*, CFont*>::iterator iter = m_mapFont.begin();
	map<const TCHAR*, CFont*>::iterator iter_end = m_mapFont.end();

	for_each(iter, iter_end, CDeleteMap());
	m_mapFont.clear();
}

Engine::CFont* Engine::CFontMgr::GetFont(const TCHAR* pFontKey)
{
	map<const TCHAR*, CFont*>::iterator iter = m_mapFont.begin();
	map<const TCHAR*, CFont*>::iterator iter_end = m_mapFont.end();

	map<const TCHAR*, CFont*>::iterator Find_iter = find_if(iter, iter_end, CTagFinder(pFontKey));

	if(Find_iter != iter_end)
	{
		return Find_iter->second;
	}

	return NULL;
}