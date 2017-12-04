/*!
 * \file SkyBox.h
 * \date 2016/07/25 15:18
 *
 * \author Jusin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
}

class CSkyBox : public Engine::CGameObject
{
private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	D3DMATERIAL9 m_MtrInfo;

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT AddComponent(void);
	void	Release(void);

private:
	CSkyBox(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkyBox(void);
};


#endif // SkyBox_h__