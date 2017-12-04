#include "NaviCell.h"
#include "Line2D.h"

Engine::CNaviCell::CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
: m_pLine(NULL)
{
	m_vPoint[Point_A] = (*pPointA);
	m_vPoint[Point_B] = (*pPointB);
	m_vPoint[Point_C] = (*pPointC);
}

Engine::CNaviCell::~CNaviCell(void)
{
	Release();
}

Engine::CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pPointA, D3DXVECTOR3* pPointB, D3DXVECTOR3* pPointC, DWORD dwIndex)
{
	CNaviCell* pCell = new CNaviCell(pPointA, pPointB, pPointC);

	if(FAILED(pCell->InitCell(pDevice, dwIndex)))
	{
		Safe_Delete(pCell);
	}

	return pCell;
}

HRESULT Engine::CNaviCell::InitCell(LPDIRECT3DDEVICE9 pDevice, DWORD dwIndex)
{
	m_pDevice = pDevice;
	m_dwIndex = dwIndex;

	ZeroMemory(m_pNeighbor, sizeof(CNaviCell*) * Neighbor_END);
	m_pLine2D[Line_AB] = CLine2D::Create(&m_vPoint[Point_A], &m_vPoint[Point_B]);
	m_pLine2D[Line_BC] = CLine2D::Create(&m_vPoint[Point_B], &m_vPoint[Point_C]);
	m_pLine2D[Line_CA] = CLine2D::Create(&m_vPoint[Point_C], &m_vPoint[Point_A]);

	HRESULT hr = D3DXCreateLine(m_pDevice, &m_pLine);

	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	return S_OK;
}

void Engine::CNaviCell::Release(void)
{
	for(int i = 0; i < Line_END; ++i)
	{
		Safe_Delete(m_pLine2D[i]);
	}

	Safe_Release(m_pLine);
}

void Engine::CNaviCell::Render(void)
{
	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3 vPoint[4];
	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];
	
	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if(vPoint[i].z < 0.f)
		{
			vPoint[i].z = 0.f;
		}

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pLine->SetWidth(2.f);
	m_pLine->Begin();
	m_pLine->DrawTransform(vPoint, Point_END + 1, &matWorld, m_Color);
	m_pLine->End();
}

bool Engine::CNaviCell::ComparePoint(D3DXVECTOR3* pFirstPoint, D3DXVECTOR3* pSecondPoint, CNaviCell* pNeighbor)
{
	if((*pFirstPoint) == m_vPoint[Point_A])
	{
		if((*pSecondPoint) == m_vPoint[Point_B])
		{
			m_pNeighbor[Neighbor_AB] = pNeighbor;

			return true;
		}
		else if((*pSecondPoint) == m_vPoint[Point_C])
		{
			m_pNeighbor[Neighbor_CA] = pNeighbor;

			return true;
		}
	}

	if((*pFirstPoint) == m_vPoint[Point_B])
	{
		if((*pSecondPoint) == m_vPoint[Point_A])
		{
			m_pNeighbor[Neighbor_AB] = pNeighbor;

			return true;
		}
		else if((*pSecondPoint) == m_vPoint[Point_C])
		{
			m_pNeighbor[Neighbor_BC] = pNeighbor;

			return true;
		}
	}

	if((*pFirstPoint) == m_vPoint[Point_C])
	{
		if((*pSecondPoint) == m_vPoint[Point_A])
		{
			m_pNeighbor[Neighbor_CA] = pNeighbor;

			return true;
		}
		else if((*pSecondPoint) == m_vPoint[Point_B])
		{
			m_pNeighbor[Neighbor_BC] = pNeighbor;

			return true;
		}
	}

	return false;
}

bool Engine::CNaviCell::CheckPass(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, eNeighbor* _pNeighbor)
{
	for(int i = 0; i < 3; ++i)
	{
		if(m_pLine2D[i]->Check_LinePass(&D3DXVECTOR2(pPos->x + pDir->x, pPos->z + pDir->z)))
		{
			*_pNeighbor = eNeighbor(i);

			return true;
		}
	}

	return false;
}

void Engine::CNaviCell::SetNeighbor(eNeighbor _Neighbor, CNaviCell* _pNeighbor)
{
	m_pNeighbor[_Neighbor] = _pNeighbor;
}	

D3DXVECTOR3* Engine::CNaviCell::GetPoint(ePoint _PointID)
{
	return &m_vPoint[_PointID];
}

Engine::CNaviCell* Engine::CNaviCell::GetNeighbor(eNeighbor _NeighborID)
{
	return m_pNeighbor[_NeighborID];
}

DWORD Engine::CNaviCell::GetIndex(void)
{
	return m_dwIndex;
}

void Engine::CNaviCell::SetColor(D3DXCOLOR _Color)
{
	m_Color = _Color;
}