/*!
 * \file Loading.h
 * \date 2016/07/16 22:04
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

#ifndef Loading_h__
#define Loading_h__

#include <process.h>

class CLoading
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	eLoadingID			m_eLoadID;
	CRITICAL_SECTION	m_CSKey;
	HANDLE				m_hThread;
	TCHAR				m_szLoadingMessage[128];
	bool				m_bComplete;

public:
	static CLoading* Create(eLoadingID eLoadID);
	static UINT WINAPI LoadingFunction(void* pArg);

public:
	HRESULT InitLoading(void);
	void	Loading_Stage1(void);
	void	Loading_Stage2(void);
	void	Loading_Stage3(void);

private:
	void Release(void);
	void NaviMeshLoading(void);

public:
	TCHAR*	GetLoadingMessage(void);
	bool	GetComplete(void);

private:
	CLoading(eLoadingID eLoadID);

public:
	~CLoading(void);
};


#endif // Loading_h__