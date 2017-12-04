#include "NaviMgr.h"
#include "NaviCell.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr(void)
:m_dwIdxCnt(0)
{

}

Engine::CNaviMgr::~CNaviMgr(void)
{
	Release();
}

HRESULT Engine::CNaviMgr::AddCell(D3DXVECTOR3* pPointA, D3DXVECTOR3* pPointB, D3DXVECTOR3* pPointC)
{
	CNaviCell* pCell = CNaviCell::Create(m_pDevice, pPointA, pPointB, pPointC, m_dwIdxCnt);
	NULL_CHECK_RETURN(pCell, E_FAIL);

	++m_dwIdxCnt;

	m_vecCell.push_back(pCell);

	return S_OK;
}

void Engine::CNaviMgr::Reserve_Cell(LPDIRECT3DDEVICE9 pDevice, DWORD dwSize)
{
	m_pDevice = pDevice;
	m_dwSize = dwSize;
	m_vecCell.reserve(dwSize);
}

void Engine::CNaviMgr::Link_Cell(void)
{
	if(m_vecCell.size() == 0)
	{
		return;
	}
	
	vector<CNaviCell*>::iterator iter = m_vecCell.begin();
	vector<CNaviCell*>::iterator iter_end = m_vecCell.end();

	for(iter; iter != iter_end; ++iter)
	{
		vector<CNaviCell*>::iterator iter_Target = m_vecCell.begin();

		while(iter_Target != m_vecCell.end())
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if((*iter_Target)->ComparePoint((*iter)->GetPoint(Point_A), (*iter)->GetPoint(Point_B), (*iter)))
			{
				(*iter)->SetNeighbor(Neighbor_AB, (*iter_Target));
			}
			else if((*iter_Target)->ComparePoint((*iter)->GetPoint(Point_B), (*iter)->GetPoint(Point_C), (*iter)))
			{
				(*iter)->SetNeighbor(Neighbor_BC, (*iter_Target));
			}
			else if((*iter_Target)->ComparePoint((*iter)->GetPoint(Point_C), (*iter)->GetPoint(Point_A), (*iter)))
			{
				(*iter)->SetNeighbor(Neighbor_CA, (*iter_Target));
			}

			++iter_Target;
		}
	}
}	

void Engine::CNaviMgr::Render(void)
{
	int iSize = m_vecCell.size();

	for(int i = 0; i < iSize; ++i)
	{
		m_vecCell[i]->Render();
	}
}

DWORD Engine::CNaviMgr::MoveOnNaviMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, DWORD dwIndex)
{
	eNeighbor	eNeighborID;
	DWORD		dwNextIndex = dwIndex;

	//라인을 통과했을 경우
	if(m_vecCell[dwIndex]->CheckPass(pPos, pDir, &eNeighborID))
	{
		CNaviCell* pNeighbor = m_vecCell[dwIndex]->GetNeighbor(eNeighborID);
		
		if(pNeighbor == NULL)
		{
			//이웃이 없을 경우
			int iA = 0;
		}
		else
		{
			dwNextIndex = pNeighbor->GetIndex();
			(*pPos) += (*pDir);
		}
	}
	else //라인을 안지나갔다.
	{
		(*pPos) += (*pDir);
	}

	return dwNextIndex;
}

void Engine::CNaviMgr::Release(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());

	m_vecCell.clear();
}

vector<Engine::CNaviCell*>* Engine::CNaviMgr::GetNaviList(void)
{
	return &m_vecCell;
}

void Engine::CNaviMgr::NaviDelete(int iIndex)
{
	vector<Engine::CNaviCell*>::iterator iter = m_vecCell.begin();

	for(int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	delete (*iter);

	iter = m_vecCell.erase(iter);
}

void Engine::CNaviMgr::NaviAllDelete(void)
{
	for(size_t i = 0; i < m_vecCell.size(); ++i)
	{
		delete m_vecCell[i];
	}
	m_vecCell.clear();
}