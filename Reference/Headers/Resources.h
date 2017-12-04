/*!
 * \file Resources.h
 * \date 2016/07/06 23:12
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

#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources : public CComponent
{
protected:
	LPDIRECT3DDEVICE9	m_pDevice;
	DWORD				m_dwRefCnt;

public:
	virtual CResources* CloneResource(void)PURE;
	virtual DWORD		Release(void)PURE;

public:
	void AddRefCnt(void);

protected:
	explicit CResources(LPDIRECT3DDEVICE9 pDevice);
	explicit CResources(const CResources& rhs);

public:
	virtual ~CResources(void);
};

END

#endif // Resources_h__