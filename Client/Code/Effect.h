/*!
 * \file Effect.h
 * \date 2016/08/12 16:44
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

#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CEffect : public Engine::CGameObject
{
protected:
	Engine::CTransform* m_pInfo;
	Engine::CVIBuffer*	m_pBuffer;
	Engine::CTexture*	m_pTexture;

protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CEffect(void);
};

#endif // Effect_h__