/*!
 * \file TimeMgr.h
 * \date 2016/07/06 23:06
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

#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;
	float				m_fTime;

public:
	float GetTime(void);

public:
	void InitTime(void);
	void SetTime(void);

private:
	CTimeMgr(void);

public:
	~CTimeMgr(void);
};

END

#endif // TimeMgr_h__