#include "2DSprite.h"

Engine::C2DSprite::C2DSprite(LPDIRECT3DDEVICE9 pDevice)
:CResources(pDevice), m_dwVectorSize(0)
{

}

Engine::C2DSprite::C2DSprite(const C2DSprite& rhs)
: CResources(rhs.m_pDevice), m_vecTexture(rhs.m_vecTexture), m_dwVectorSize(rhs.m_dwVectorSize)
{
	++m_dwRefCnt;
}

Engine::C2DSprite::~C2DSprite(void)
{

}

Engine::C2DSprite* Engine::C2DSprite::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, WORD wCnt)
{
	C2DSprite* pSprite = new C2DSprite(pDevice);

	if(FAILED(pSprite->LoadTexture(pFilePath, wCnt)))
	{
		Engine::Safe_Delete(pSprite);
	}

	return pSprite;
}

HRESULT Engine::C2DSprite::LoadTexture(const TCHAR* pFilePath, WORD wCnt)
{	
	TCHAR szFullPath[MAX_PATH] = L"";

	for(int i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, pFilePath, i);
		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szFullPath, &pTexInfo->ImgInfo)))
		{
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice, szFullPath, pTexInfo->ImgInfo.Width, pTexInfo->ImgInfo.Height, pTexInfo->ImgInfo.MipLevels, 0, pTexInfo->ImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &pTexInfo->ImgInfo, NULL, &pTexInfo->pTexture)))
		{
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}
		
		m_vecTexture.push_back(pTexInfo);
	}

	return S_OK;
}

void Engine::C2DSprite::Render(WORD wIndex /*= 0*/)
{
	
}

Engine::CResources* Engine::C2DSprite::CloneResource(void)
{
	return new C2DSprite(*this);
}

DWORD Engine::C2DSprite::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		DWORD dwSize = m_vecTexture.size();

		for(DWORD i = 0; i < dwSize; ++i)
		{
			Engine::Safe_Release(m_vecTexture[i]->pTexture);
			Engine::Safe_Delete(m_vecTexture[i]);
		}
		m_vecTexture.clear();

		return 0;
	}
	else
		--m_dwRefCnt;

	return 0;
}

Engine::TEXINFO* Engine::C2DSprite::GetTexture(WORD wCnt)
{
	return m_vecTexture[wCnt];
}