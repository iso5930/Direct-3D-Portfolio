/*!
 * \file LogoBack.h
 * \date 2016/07/08 13:02
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

#ifndef LogoBack_h__
#define LogoBack_h__

#include "GameObject.h"

namespace Engine
{
	class CResourcsMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
}

class CLogoBack : public Engine::CGameObject
{
private:
	Engine::CResourcsMgr*	m_pResourceMgr;
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	D3DMATERIAL9		m_MtrInfo;

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int	Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	void	Release(void);

private:
	CLogoBack(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogoBack(void);
};

#endif // LogoBack_h__