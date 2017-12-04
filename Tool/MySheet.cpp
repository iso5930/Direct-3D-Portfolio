// MySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MySheet.h"

// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet(void)
{
	AddPage(&m_NaviPage);
	AddPage(&m_CollisionPage);
}

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CMySheet::~CMySheet()
{
	
}

BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMySheet 메시지 처리기입니다.

BOOL CMySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  여기에 특수화된 코드를 추가합니다.

	SetActivePage(&m_NaviPage);
	SetActivePage(&m_CollisionPage);
	SetActivePage(&m_NaviPage);

	return bResult;
}