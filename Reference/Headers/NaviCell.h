/*!
 * \file NaviCell.h
 * \date 2016/07/26 22:34
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

#ifndef NaviCell_h__
#define NaviCell_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLine2D;

class ENGINE_DLL CNaviCell
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	CNaviCell*			m_pNeighbor[Neighbor_END];
	CLine2D*			m_pLine2D[Line_END];

private:
	D3DXVECTOR3			m_vPoint[Point_END];
	LPD3DXLINE			m_pLine;
	DWORD				m_dwIndex;
	D3DXCOLOR			m_Color;

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pPointA, D3DXVECTOR3* pPointB, D3DXVECTOR3* pPointC, DWORD dwIndex);

private:
	HRESULT InitCell(LPDIRECT3DDEVICE9 pDevice, DWORD dwIndex);
	void	Release(void);

public:
	void Render(void);

public:
	bool	ComparePoint(D3DXVECTOR3* pFirstPoint, D3DXVECTOR3* pSecondPoint, CNaviCell* pNeighbor);
	bool	CheckPass(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, eNeighbor* _pNeighbor);

public:
	void SetNeighbor(eNeighbor _Neighbor, CNaviCell* _pNeighbor);
	void SetColor(D3DXCOLOR _Color);

public:
	D3DXVECTOR3*	GetPoint(ePoint _PointID);
	CNaviCell*		GetNeighbor(eNeighbor _NeighborID);
	DWORD			GetIndex(void);

private:
	explicit CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);

public:
	~CNaviCell(void);
};

END

#endif // NaviCell_h__