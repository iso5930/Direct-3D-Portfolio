#pragma once
#include "afxwin.h"


// CNaviPage ��ȭ �����Դϴ�.

class CNaviPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNaviPage)

public:
	vector<D3DXVECTOR3> m_vecPoint;

public:
	void Update(void);
	
public:
	CNaviPage();
	virtual ~CNaviPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NAVIPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_iViewMode;
	void OnBnClickedRadio(UINT msg);
	CListBox m_NaviMeshList;
	CString m_szPointA;
	CString m_szPointB;
	CString m_szPointC;
	afx_msg void OnBnClickedDeletebutton();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLbnSelchangeNavimeshlist();
	afx_msg void OnBnClickedButton1();

	virtual BOOL OnSetActive();

};