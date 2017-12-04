// SheetView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "SheetView.h"
#include "MySheet.h"

#include "Engine_Include.h"

// CSheetView

IMPLEMENT_DYNCREATE(CSheetView, CView)

CSheetView::CSheetView()
{
	m_pMySheet = NULL;
}

CSheetView::~CSheetView()
{
	Engine::Safe_Delete(m_pMySheet);
}

BEGIN_MESSAGE_MAP(CSheetView, CView)
END_MESSAGE_MAP()


// CSheetView �׸����Դϴ�.

void CSheetView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}

// CSheetView �����Դϴ�.

#ifdef _DEBUG
void CSheetView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSheetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSheetView �޽��� ó�����Դϴ�.

void CSheetView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	
	m_pMySheet->MoveWindow(0, 0, 350, WINCY);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}