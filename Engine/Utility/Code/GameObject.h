/*!
 * \file GameObject.h
 * \date 2016/07/01 23:01
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

#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject
{
protected:
	map<const TCHAR*, CComponent*>	m_mapComponent;
	LPDIRECT3DDEVICE9				m_pDevice;

public:
	virtual HRESULT Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);

private:
	void Release(void);

public:
	CComponent* GetComponent(TCHAR* pComponentKey);

protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGameObject(void);
};

END

#endif // GameObject_h__