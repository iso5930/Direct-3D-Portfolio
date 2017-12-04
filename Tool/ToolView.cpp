
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "Export_Function.h"
#include "BackGround.h"
#include "Camera_Tool.h"
#include "StaticMesh.h"
#include "SheetView.h"
#include "MySheet.h"
#include "Sphere.h"
#include "Transform.h"
#include "NaviCell.h"
#include "Character.h"
#include "Collision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pDevice = NULL;
	m_pCamera = NULL;
}

CToolView::~CToolView()
{
	Engine::Get_InputMgr()->DestroyInstance();
	Engine::Get_TimeMgr()->DestroyInstance();
	Engine::Get_GraphicDev()->DestroyInstance();
	Engine::Get_ResourceMgr()->DestroyInstance();
	Engine::Get_NaviMgr()->DestroyInstance();
	Engine::Safe_Delete(m_pBack);
	Engine::Safe_Delete(m_pCamera);

	for(size_t i = 0; i < m_vecSphere.size(); ++i)
	{
		delete m_vecSphere[i];
	}

	m_vecSphere.clear();

	Engine::Safe_Delete(m_pCharacter);
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

void CToolView::Progress(void)
{
	if(m_iIndex == 0)
	{
		m_pBack->Update();

		for(size_t i = 0; i < m_vecSphere.size(); ++i)
		{
			m_vecSphere[i]->Update();
		}
	}
	else if(m_iIndex == 1)
	{
		m_pCharacter->Update();

		vector<Engine::CCollision*> vecCol = ((CMainFrame*)AfxGetMainWnd())->m_pSheetView->m_pMySheet->m_CollisionPage.m_vecColision;

		for(size_t i = 0; i < vecCol.size(); ++i)
		{
			vecCol[i]->Update();
		}
	}
	
	m_pCamera->Update();	
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	if(m_iIndex == 0)
	{
		m_pBack->Render();

		for(size_t i = 0; i < m_vecSphere.size(); ++i)
		{
			m_vecSphere[i]->Render();
		}

		Engine::Get_NaviMgr()->Render();
	}
	else if(m_iIndex == 1)
	{
		m_pCharacter->Render();

		vector<Engine::CCollision*> vecCol = ((CMainFrame*)AfxGetMainWnd())->m_pSheetView->m_pMySheet->m_CollisionPage.m_vecColision;

		for(size_t i = 0; i < vecCol.size(); ++i)
		{
			vecCol[i]->Render();
		}
	}

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

// CToolView �μ�


void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;

	pFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pFrame->SetWindowPos(NULL, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	g_hWnd = m_hWnd;

	Engine::Get_GraphicDev()->InitGraphicDev(Engine::CGraphicDevice::MODE_WIN, g_hWnd, WINCX, WINCY);

	m_pDevice = Engine::Get_GraphicDev()->GetDevice();

	HRESULT hr = NULL;

	hr = Engine::Get_ResourceMgr()->ReserveContainerSize(Resource_End);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"1Stage_Map", L"../Client/bin/Resource/Mesh/Map/1Stage/", L"1Stage.X");

	m_pBack = new CBackGround(m_pDevice);

	m_pBack->Initialize();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_pMainView = this;

	m_pCamera = new CCamera_Tool(m_pDevice);

	m_pCamera->Initialize(D3DXVECTOR3(0.f, 5.f, -20.f), D3DXVECTOR3(0.f, 0.f, 0.f));

	Engine::Get_InputMgr()->InitInputDevice(AfxGetInstanceHandle(), g_hWnd);

	Engine::Get_TimeMgr()->InitTime();

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pCharacter = new CCharacter(m_pDevice);
	m_pCharacter->Initialize();

	Engine::Get_NaviMgr()->Reserve_Cell(m_pDevice, 100);
}
void CToolView::OnDestroy()
{
	CView::OnDestroy();
}
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_iIndex != 0)
	{
		return;
	}

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	float	fU = 0.f;
	float	fV = 0.f;
	float	fDist = 0.f;

	DWORD dwCnt = 0;
	DWORD dwIndex = 0;

	D3DXMATRIX matProj;
	POINT ptCursor;

	D3DVIEWPORT9 ViewPortInfo;
	m_pDevice->GetViewport(&ViewPortInfo);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * ptCursor.x ) / ViewPortInfo.Width  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * ptCursor.y ) / ViewPortInfo.Height ) - 1 ) / matProj._22;
	v.z =  1.0f;

	D3DXMATRIX matView, m;
	m_pDevice->GetTransform( D3DTS_VIEW, &matView );
	D3DXMatrixInverse( &m, NULL, &matView );

	D3DXVECTOR3 vRayDir(v.x, v.y, 1);
	D3DXVECTOR3 vRayOrig(0, 0, 0);

	D3DXVec3TransformCoord(&vRayOrig, &vRayOrig, &m);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &m);

	BOOL bCheck = false;

	Engine::CStaticMesh* pMesh = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_pMesh;

	HRESULT hr = NULL;

	hr = D3DXIntersect(pMesh->m_pMesh, &vRayOrig, &vRayDir, &bCheck, &dwIndex, &fU, &fV, &fDist, &pMesh->m_pSubset, &dwCnt);
	
	if(bCheck == TRUE)
	{
		vector<D3DXVECTOR3>* vecPoint = &(((CMainFrame*)AfxGetMainWnd())->m_pSheetView->m_pMySheet->m_NaviPage.m_vecPoint);

		D3DXVECTOR3 vPoint = vRayOrig + vRayDir * fDist;
		//���� �̳༮�� ������ ���ָ�...

		bool bCheck2 = false;

		for(size_t i = 0; i < m_vecSphere.size(); ++i)
		{
			D3DXVECTOR3 vDist = vPoint - m_vecSphere[i]->m_pInfo->m_vPos;

			float fDist = abs(D3DXVec3Length(&vDist));

			if(fDist < 20.f)
			{
				bCheck2 = true;

				vecPoint->push_back(m_vecSphere[i]->m_pInfo->m_vPos);

				break;
			}
		}
		if(bCheck2 == false)
		{
			vecPoint->push_back(vPoint);

			CSphere* pSphere = new CSphere(m_pDevice);

			pSphere->Initialize(vPoint);

			m_vecSphere.push_back(pSphere);
		}

		int iSize = vecPoint->size();

		if(iSize == 3)
		{
			Engine::Get_NaviMgr()->AddCell(&(*vecPoint)[0], &(*vecPoint)[1], &(*vecPoint)[2]);
			vecPoint->clear();
		}

		((CMainFrame*)AfxGetMainWnd())->m_pSheetView->m_pMySheet->m_NaviPage.Update();
	}
	
	CView::OnLButtonDown(nFlags, point);
}

void CToolView::SphereReset(void)
{
	for(size_t i = 0; i < m_vecSphere.size(); ++i)
	{
		delete m_vecSphere[i];
	}
	m_vecSphere.clear();

	//�ϴ��ѹ��� �����ְ�..

	//�׺�޽� �ٽ� ����...

	vector<Engine::CNaviCell*>* vecNaviCell = Engine::Get_NaviMgr()->GetNaviList();

	for(size_t i  = 0; i < vecNaviCell->size(); ++i)
	{
		D3DXVECTOR3 vPoint[3];
		bool		bCheck[3] = {false};
		
		vPoint[0] = *((*vecNaviCell)[i])->GetPoint(Point_A);
		vPoint[1] = *((*vecNaviCell)[i])->GetPoint(Point_B);
		vPoint[2] = *((*vecNaviCell)[i])->GetPoint(Point_C);

		for(size_t j = 0; j < m_vecSphere.size(); ++j)
		{
			if(m_vecSphere[j]->m_pInfo->m_vPos == vPoint[0])
			{
				bCheck[0] = true;
			}
			if(m_vecSphere[j]->m_pInfo->m_vPos == vPoint[1])
			{
				bCheck[1] = true;
			}
			if(m_vecSphere[j]->m_pInfo->m_vPos == vPoint[2])
			{
				bCheck[2] = true;
			}
		}

		for(int j = 0; j < 3; ++j)
		{
			if(bCheck[j] == false)
			{
				CSphere* pSphere = new CSphere(m_pDevice);

				pSphere->Initialize(vPoint[j]);

				m_vecSphere.push_back(pSphere);
			}
		}
	}
}