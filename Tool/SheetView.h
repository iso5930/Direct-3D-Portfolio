#pragma once


// CSheetView ���Դϴ�.

class CMySheet;
class CSheetView : public CView
{
	DECLARE_DYNCREATE(CSheetView)

public:
	CMySheet* m_pMySheet;

protected:
	CSheetView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSheetView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};