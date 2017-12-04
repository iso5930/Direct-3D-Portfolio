/*!
 * \file Component.h
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

#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
public:
	virtual int		Update(void);
	virtual DWORD	Release(void)PURE;

protected:
	explicit CComponent(void);

public:
	virtual ~CComponent(void);
};

END

#endif // Component_h__