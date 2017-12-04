// CollisionPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "CollisionPage.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Character.h"
#include "DynamicMesh.h"
#include "Collision.h"
#include "Transform.h"
#include "Export_Function.h"

// CCollisionPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCollisionPage, CPropertyPage)

CCollisionPage::CCollisionPage()
	: CPropertyPage(CCollisionPage::IDD)
	, m_iPosX(0)
	, m_iPosY(0)
	, m_iPosZ(0)
	, m_iScaleX(0)
	, m_iScaleY(0)
	, m_iScaleZ(0)
	, m_pCollision(NULL)
	, m_iAniIndex(0)
{

}

CCollisionPage::~CCollisionPage()
{
	Release();
}

void CCollisionPage::Release(void)
{
	for(size_t i = 0; i < m_vecColision.size(); ++i)
	{
		Safe_Delete(m_vecColision[i]);
	}
	m_vecColision.clear();
}

void CCollisionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, ICD_BONE_LIST, m_BoneList);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_iPosX);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_iPosY);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_iPosZ);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_iScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_iScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALE_Z, m_iScaleZ);
	DDX_Text(pDX, IDC_EDIT1, m_iAniIndex);
}


BOOL CCollisionPage::OnSetActive()
{
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_iIndex = 1;

	//������ �ݸ��� ������

	return CPropertyPage::OnSetActive();
}

BEGIN_MESSAGE_MAP(CCollisionPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON2, &CCollisionPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CCollisionPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CCollisionPage::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCollisionPage �޽��� ó�����Դϴ�.
BOOL CCollisionPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

// 	m_BoneList.AddString(L"ValveBiped_Bip01_Pelvis"); //��ü
// 	m_BoneList.AddString(L"ValveBiped_Bip01_Spine"); //��ü
// 	m_BoneList.AddString(L"ValveBiped_Bip01_Spine1"); //��ü
// 	m_BoneList.AddString(L"ValveBiped_Bip01_Spine2"); //��ü
// 	m_BoneList.AddString(L"ValveBiped_Bip01_Spine4"); //��ü
// 	//m_BoneList.AddString(L"ValveBiped_Bip01_Head");//�Ӹ�
// 	m_BoneList.AddString(L"ValveBiped_Bip01_Head1");//�Ӹ�
// 
// 	m_BoneList.AddString(L"ValveBiped_Anim_Attachment_LH");//����
// 	m_BoneList.AddString(L"ValveBiped_Anim_Attachment_RH");//����
// 
//  	m_BoneList.AddString(L"ValveBiped_Bip01_L_UpperArm"); //��
//  	m_BoneList.AddString(L"ValveBiped_Bip01_R_UpperArm"); //��
// 	m_BoneList.AddString(L"ValveBiped_Bip01_L_ForeTwist"); //��
// 	m_BoneList.AddString(L"ValveBiped_Bip01_R_ForeTwist"); //��
// 
// 	m_BoneList.AddString(L"ValveBiped_Bip01_L_Shoulder01"); //��
// 	m_BoneList.AddString(L"ValveBiped_Bip01_R_Shoulder01"); //��
// 
// 	m_BoneList.AddString(L"ValveBiped_Bip01_L_Hand"); //��
// 	m_BoneList.AddString(L"ValveBiped_Bip01_R_Hand"); //��
// 	
// 	m_BoneList.AddString(L"ValveBiped_Bip01_L_Calf"); //���Ƹ�
// 	m_BoneList.AddString(L"ValveBiped_Bip01_R_Calf"); //���Ƹ�
// 
// 	m_BoneList.AddString(L"ValveBiped_Bip01_L_Thigh"); //�����
// 	m_BoneList.AddString(L"ValveBiped_Bip01_R_Thigh"); //�����
// 
// 	m_BoneList.AddString(L"ValveBiped_Bip01_R_Foot"); //��
// 	m_BoneList.AddString(L"ValveBiped_Bip01_L_Foot"); //��

	//��ų

	m_BoneList.AddString(L"Object032_Bone");
	m_BoneList.AddString(L"Object033_Bone");
	m_BoneList.AddString(L"Object034_Bone");
	m_BoneList.AddString(L"Object035_Bone");
	m_BoneList.AddString(L"Object036_Bone");
	m_BoneList.AddString(L"Object037_Bone");
	m_BoneList.AddString(L"Object038_Bone");
	m_BoneList.AddString(L"Object039_Bone");
	m_BoneList.AddString(L"Object040_Bone");
	m_BoneList.AddString(L"Object041_Bone");
	m_BoneList.AddString(L"Object042_Bone");
	m_BoneList.AddString(L"Object043_Bone");
	m_BoneList.AddString(L"Object044_Bone");
	m_BoneList.AddString(L"Object045_Bone");
	m_BoneList.AddString(L"Object046_Bone");
	m_BoneList.AddString(L"Object047_Bone");
	m_BoneList.AddString(L"Object048_Bone");
	m_BoneList.AddString(L"Object049_Bone");
	m_BoneList.AddString(L"Object050_Bone");
	m_BoneList.AddString(L"Object051_Bone");
	m_BoneList.AddString(L"Object052_Bone");
	m_BoneList.AddString(L"Object053_Bone");
	m_BoneList.AddString(L"Object054_Bone");
	m_BoneList.AddString(L"Object055_Bone");
	m_BoneList.AddString(L"Object056_Bone");
	m_BoneList.AddString(L"Object057_Bone");
	m_BoneList.AddString(L"Object058_Bone");
	m_BoneList.AddString(L"Object059_Bone");
	m_BoneList.AddString(L"Object060_Bone");
	m_BoneList.AddString(L"Object061_Bone");
	m_BoneList.AddString(L"Object062_Bone");
	m_BoneList.AddString(L"Object063_Bone");
	m_BoneList.AddString(L"Object064_Bone");
	m_BoneList.AddString(L"Object065_Bone");
	m_BoneList.AddString(L"Object066_Bone");
	m_BoneList.AddString(L"Object067_Bone");
	m_BoneList.AddString(L"Object068_Bone");
	m_BoneList.AddString(L"Object069_Bone");
	m_BoneList.AddString(L"Object070_Bone");
	m_BoneList.AddString(L"Object071_Bone");
	m_BoneList.AddString(L"Object072_Bone");
	m_BoneList.AddString(L"Object073_Bone");
	m_BoneList.AddString(L"Object074_Bone");
	m_BoneList.AddString(L"Object075_Bone");
	m_BoneList.AddString(L"Object076_Bone");
	m_BoneList.AddString(L"Object077_Bone");
	m_BoneList.AddString(L"Object078_Bone");

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
void CCollisionPage::OnBnClickedButton2()
{
	//����
	UpdateData(TRUE);

	int iIndex = m_BoneList.GetCurSel();

	TCHAR szName[MAX_PATH] = L"";

	m_BoneList.GetText(iIndex, szName);

	char szTemp[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szName, MAX_PATH, szTemp, MAX_PATH, NULL, NULL);

	const D3DXMATRIX* matInfo = &(((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pCharacter->m_pInfo->m_matWorld);
	const D3DXMATRIX* matCombined = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pCharacter->m_pMesh->FindFrame(szTemp);

	LPDIRECT3DDEVICE9 pDevice = Engine::Get_GraphicDev()->GetDevice();

	//�ι� �����ٶ� ������ �����... 

	m_pCollision = Engine::CCollision::Create(pDevice, 30.f, matInfo, matCombined);

	m_vecColision.push_back(m_pCollision);

	UpdateData(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CCollisionPage::OnBnClickedButton1()
{
	//����
	UpdateData(TRUE);

	D3DXVECTOR3 vPos((float)m_iPosX, (float)m_iPosY, (float)m_iPosZ);
	D3DXVECTOR3 vScale((float)m_iScaleX, (float)m_iScaleY, (float)m_iScaleZ);

	m_pCollision->SetPos(vPos);
	m_pCollision->SetScale(vScale);

	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
void CCollisionPage::OnBnClickedButton3()
{
	UpdateData(TRUE);
	
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pCharacter->m_pMesh2->SetAnimation(m_iAniIndex);
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pCharacter->m_pMesh2->SetTrackPos(0.0);

	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}