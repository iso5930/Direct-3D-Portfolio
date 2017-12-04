// NaviPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviPage.h"
#include "Export_Function.h"
#include "NaviCell.h"
#include "MainFrm.h"
#include "ToolView.h"

// CNaviPage ��ȭ �����Դϴ�.

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

// CNaviPage �޽��� ó�����Դϴ�.

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

				strNaviName.Format(L"%d%s", i, L"��");

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	
	int iCur = m_NaviMeshList.GetCurSel();

	if(iCur == -1)
	{
		return;
	}
	
	Engine::Get_NaviMgr()->NaviDelete(iCur);

	m_NaviMeshList.DeleteString(iCur);

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->SphereReset(); //���Ǿ� �ٽü���.

	UpdateData(FALSE);
}
void CNaviPage::OnBnClickedButton3()
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../Client/bin/Data/NaviData.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"���� ����");
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

	AfxMessageBox(L"���� ����");

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
void CNaviPage::OnBnClickedButton4()
{
	vector<Engine::CNaviCell*>* vecNavi = Engine::Get_NaviMgr()->GetNaviList();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../Client/bin/Data/NaviData.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"�ε� ����");
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

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->SphereReset(); //���Ǿ� �ٽü���.

	AfxMessageBox(L"�ε� ����");
}
void CNaviPage::OnLbnSelchangeNavimeshlist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		((*vecNavi)[i])->SetColor(D3DXCOLOR(1.f, 0.f, 0.f, 1.f)); //�� �ʱ�ȭ
	}

	((*vecNavi)[iCur])->SetColor(D3DXCOLOR(0.f, 1.f, 1.f, 1.f));

	//�̿� �ش��ϴ¼��� �̿��� ���θ� Ȯ������...

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
	//������ �׺� ������..

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_iIndex = 0;


	return CPropertyPage::OnSetActive();
}