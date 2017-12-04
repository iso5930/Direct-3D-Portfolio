#pragma once
#include "afxwin.h"

// CCollisionPage 대화 상자입니다.

namespace Engine
{
	class CCollision;
}

class CCollisionPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCollisionPage)
public:
	vector<Engine::CCollision*> m_vecColision;
	Engine::CCollision*			m_pCollision;

public:
	CCollisionPage();
	virtual ~CCollisionPage();

public:
	void Release(void);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COLLISIONPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnSetActive();

public:
	virtual BOOL OnInitDialog();
	CListBox m_BoneList;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	int m_iPosX;
	int m_iPosY;
	int m_iPosZ;
	int m_iScaleX;
	int m_iScaleY;
	int m_iScaleZ;
	int m_iAniIndex;
	afx_msg void OnBnClickedButton3();
};
