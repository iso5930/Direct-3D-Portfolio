// NaviPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviPage.h"
#include "Export_Function.h"
#include "NaviCell.h"
#include "MainFrm.h"
#include "ToolView.h"

// CNaviPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviPage, CPropertyPage)

CNaviPage::CNaviPage()
	: CPropertyPage(CNaviPage::IDD)
	, m_iViewMode(0)
	, m_szPointA(_T(""))
	, m_szPointB(_T(""))
	, m_szPointC(_T(""))
{
	Engine::Get_GraphicDev()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

CNaviPage::~CNaviPage()
{

}

void CNaviPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, ViewMode1, m_iViewMode);
	DDX_Control(pDX, NaviMeshList, m_NaviMeshList);
	DDX_Text(pDX, IDC_PointA, m_szPointA);
	DDX_Text(pDX, IDC_PointB, m_szPointB);
	DDX_Text(pDX, IDC_PointC, m_szPointC);
}


BEGIN_MESSAGE_MAP(CNaviPage, CPropertyPage)

	ON_CONTROL_RANGE(BN_CLICKED, ViewMode1, ViewMode2, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_DeleteButton, &CNaviPage::OnBnClickedDeletebutton)
	ON_BN_CLICKED(IDC_BUTTON3, &CNaviPage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNaviPage::OnBnClickedButton4)
	ON_LBN_SELCHANGE(NaviMeshList, &CNaviPage::OnLbnSelchangeNavimeshlist)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviPage::OnBnClickedButton1)
END_MESSAGE_MAP()

// CNaviPage 메시지 처리기입니다.

void CNaviPage::OnBnClickedRadio(UINT msg)
{
	UpdateData(TRUE);

	switch(m_iViewMode)
	{
	case 0:
		Engine::Get_GraphicDev()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		break;

	case 1:
		Engine::Get_GraphicDev()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;
	}

	UpdateData(FALSE);
}

void CNaviPage::Update(void)
{
	UpdateData(TRUE);

	if(m_vecPoint.size() == 0)
	{
		m_szPointA = L"";
		m_szPointB = L"";
		m_szPointC = L"";

		vector<Engine::CNaviCell*>* vecNavi = Engine::Get_NaviMgr()->GetNaviList();
		
		int iSize = vecNavi->size();

		int iCount = m_NaviMeshList.GetCount();

		if(iSize != iCount)
		{
			for(int i = iCount; i < iSize; ++i)
			{
				CString strNaviName = L"";

				strNaviName.Format(L"%d%s", i, L"번");

				m_NaviMeshList.AddString(strNaviName);
			}
		}
	}

	int iX, iY, iZ;

	int iSize = m_vecPoint.size();

	for(int i = 0; i < iSize; ++i)
	{
		iX = (int)m_vecPoint[i].x;
		iY = (int)m_vecPoint[i].y;
		iZ = (int)m_vecPoint[i].z;

		CString strA, strB, strC;

		strA.Format(L"%s%d", L" X : ", iX);
		strB.Format(L"%s%d", L" Y : ", iY);
		strC.Format(L"%s%d", L" Z : ", iZ);
		
		switch(i)
		{
		case 0:
			m_szPointA = strA + strB + strC;
			break; 

		case 1:
			m_szPointB = strA + strB + strC;
			break;

		case 2:
			m_szPointC = strA + strB + strC;
			break;
		}
	}

	UpdateData(FALSE);
}
void CNaviPage::OnBnClickedDeletebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	int iCur = m_NaviMeshList.GetCurSel();

	if(iCur == -1)
	{
		return;
	}
	
	Engine::Get_NaviMgr()->NaviDelete(iCur);

	m_NaviMeshList.DeleteString(iCur);

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->SphereReset(); //스피어 다시설정.

	UpdateData(FALSE);
}
void CNaviPage::OnBnClickedButton3()
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../Client/bin/Data/NaviData.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"저장 실패");
	}

	vector<Engine::CNaviCell*>* vecNavi = Engine::Get_NaviMgr()->GetNaviList();

	int iSize = vecNavi->size();

	WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i < iSize; ++i)
	{
		D3DXVECTOR3 vPoint[3];
		vPoint[0] = *((*vecNavi)[i]->GetPoint(Point_A));
		vPoint[1] = *((*vecNavi)[i]->GetPoint(Point_B));
		vPoint[2] = *((*vecNavi)[i]->GetPoint(Point_C));

		WriteFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);
	}
	
	CloseHandle(hFile);

	AfxMessageBox(L"저장 성공");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CNaviPage::OnBnClickedButton4()
{
	vector<Engine::CNaviCell*>* vecNavi = Engine::Get_NaviMgr()->GetNaviList();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../Client/bin/Data/NaviData.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"로드 실패");
	}

	Engine::Get_NaviMgr()->NaviAllDelete();

	int iSize = 0;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	while(true)
	{
		D3DXVECTOR3 vPoint[3];

		ReadFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);

		if(dwByte == 0)
		{
			break;
		}
		
		Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	}

	CloseHandle(hFile);

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->SphereReset(); //스피어 다시설정.

	AfxMessageBox(L"로드 성공");
}
void CNaviPage::OnLbnSelchangeNavimeshlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iCur = m_NaviMeshList.GetCurSel();

	if(iCur == -1)
	{
		UpdateData(FALSE);
		return;
	}

	vector<Engine::CNaviCell*>* vecNavi = Engine::Get_NaviMgr()->GetNaviList();
	
	for(size_t i = 0; i < vecNavi->size(); ++i)
	{
		((*vecNavi)[i])->SetColor(D3DXCOLOR(1.f, 0.f, 0.f, 1.f)); //색 초기화
	}

	((*vecNavi)[iCur])->SetColor(D3DXCOLOR(0.f, 1.f, 1.f, 1.f));

	//이웃 해당하는셀의 이웃의 여부를 확인하자...

	CString strCheck[3] = {L""};

	for(int i = 0; i < 3; ++i)
	{
		if(((*vecNavi)[iCur])->GetNeighbor(eNeighbor(i)) != NULL)
		{
			strCheck[i] = L"O";
		}
	}

	SetDlgItemText(IDC_STATIC_AB, strCheck[0]);
	SetDlgItemText(IDC_STATIC_BC, strCheck[1]);
	SetDlgItemText(IDC_STATIC_CA, strCheck[2]);

	UpdateData(FALSE);
}
void CNaviPage::OnBnClickedButton1()
{
	Engine::Get_NaviMgr()->Link_Cell();
}

BOOL CNaviPage::OnSetActive()
{
	//지금은 네비 페이지..

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_iIndex = 0;


	return CPropertyPage::OnSetActive();
}