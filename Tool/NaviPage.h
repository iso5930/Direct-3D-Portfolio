#pragma once
#include "afxwin.h"


// CNaviPage 대화 상자입니다.

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

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NAVIPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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