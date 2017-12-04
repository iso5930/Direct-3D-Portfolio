#pragma once
#include "afxwin.h"

// CCollisionPage ��ȭ �����Դϴ�.

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

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_COLLISIONPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
