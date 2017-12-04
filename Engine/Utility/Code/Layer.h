/*!
 * \file Layer.h
 * \date 2016/07/01 22:59
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

#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer
{
private:
	map<const TCHAR*, list<CGameObject*>>	m_mapObjList;
	LPDIRECT3DDEVICE9						m_pDevice;

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	HRESULT AddObject(const TCHAR* pObjKey, CGameObject* pGameObject);

public:
	int		Update(void);
	void	Render(void);

private:
	void	Release(void);

public:
	CComponent* GetComponent(TCHAR* pObjKey, TCHAR* pComponentKey);
	CGameObject* GetGameObject(TCHAR* pObjKey);

private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLayer(void);
};

END

#endif // Layer_h__