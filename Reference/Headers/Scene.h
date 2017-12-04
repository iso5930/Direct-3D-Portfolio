/*!
 * \file Scene.h
 * \date 2016/07/01 22:53
 *
 * \author Jin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;

class ENGINE_DLL CScene
{
protected:
	LPDIRECT3DDEVICE9	m_pDevice;
	map<WORD, CLayer*>	m_mapLayer;

public:
	virtual HRESULT Initialize(void)PURE;
	virtual	void	Update(void);
	virtual	void	Render(void);

private:
	void Release(void);

public:
	CComponent* GetComponent(WORD LayerID, TCHAR* pObjKey, TCHAR* pComponentKey);
	CGameObject*	GetGameObject(WORD LayerID, TCHAR* pObjKey);

protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CScene(void);
};

END

#endif // Scene_h__