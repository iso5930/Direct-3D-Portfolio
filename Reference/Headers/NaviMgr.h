/*!
 * \file NaviMgr.h
 * \date 2016/07/27 16:35
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

#ifndef NaviMgr_h__
#define NaviMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CNaviCell;
class ENGINE_DLL CNaviMgr
{
public:
	DECLARE_SINGLETON(CNaviMgr);

private:
	vector<CNaviCell*>	m_vecCell;
	LPDIRECT3DDEVICE9	m_pDevice;
	DWORD				m_dwSize;
	DWORD				m_dwIdxCnt;

public:
	HRESULT AddCell(D3DXVECTOR3* pPointA, D3DXVECTOR3* pPointB, D3DXVECTOR3* pPointC);
	void	Reserve_Cell(LPDIRECT3DDEVICE9 pDevice, DWORD dwSize);
	void	Link_Cell(void);
	void	Render(void);
	DWORD	MoveOnNaviMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, DWORD dwIndex);

public:
	//Tools Àü¿ë
	vector<CNaviCell*>* GetNaviList(void);
	void	NaviDelete(int iIndex);
	void	NaviAllDelete(void);

private:
	void Release(void);

private:
	CNaviMgr(void);
	~CNaviMgr(void);
};

END

#endif // NaviMgr_h__