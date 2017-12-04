#include "stdafx.h"
#include "CustomColorUI.h"
#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Player.h"

CCustomColor_UI::CCustomColor_UI(LPDIRECT3DDEVICE9 pDevice)
:Engine::CGameObject(pDevice), m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCustomColor_UI::~CCustomColor_UI(void)
{

}

CCustomColor_UI* CCustomColor_UI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCustomColor_UI*		pGameObject = new CCustomColor_UI(pDevice);

	if(FAILED(pGameObject->Initialize()))
	{
		Engine::Safe_Delete(pGameObject);
	}

	return pGameObject;
}

HRESULT CCustomColor_UI::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fSizeX = 231.f / 2.f;
	m_fSizeY = 386.f / 2.f;
	m_fX = WINCX - (231 / 2.f);
	m_fY = WINCY / 2.f;

	g_bCustomColor = false;

	float fX;
	float fY;
	float fSizeX;
	float fSizeY;

	fX = m_fX - 85.f;
	fY = m_fY - 132.f;

	fSizeX = 44.f;
	fSizeY = 48.f;

	m_rcCustomParts[0].left	= long(fX - fSizeX / 2.f);
	m_rcCustomParts[0].top	= long(fY - fSizeY / 2.f);
	m_rcCustomParts[0].right	= long(fX + fSizeX / 2.f);
	m_rcCustomParts[0].bottom = long(fY + fSizeY / 2.f);

	fX = m_fX - 30.f;
	m_rcCustomParts[1].left	= long(fX - fSizeX / 2.f);
	m_rcCustomParts[1].top	= long(fY - fSizeY / 2.f);
	m_rcCustomParts[1].right	= long(fX + fSizeX / 2.f);
	m_rcCustomParts[1].bottom = long(fY + fSizeY / 2.f);

	fX = m_fX + 27.f;
	m_rcCustomParts[2].left	= long(fX - fSizeX / 2.f);
	m_rcCustomParts[2].top	= long(fY - fSizeY / 2.f);
	m_rcCustomParts[2].right	= long(fX + fSizeX / 2.f);
	m_rcCustomParts[2].bottom = long(fY + fSizeY / 2.f);

	fX = m_fX + 83.f;
	m_rcCustomParts[3].left	= long(fX - fSizeX / 2.f);
	m_rcCustomParts[3].top	= long(fY - fSizeY / 2.f);
	m_rcCustomParts[3].right	= long(fX + fSizeX / 2.f);
	m_rcCustomParts[3].bottom = long(fY + fSizeY / 2.f);


	fY = m_fY - 88.f;
	fX = m_fX - 76.f;
	fSizeX = 60.f;
	fSizeY = 30.f;

	m_rcCustomColor[0].left	= long(fX - fSizeX / 2.f);
	m_rcCustomColor[0].top	= long(fY - fSizeY / 2.f);
	m_rcCustomColor[0].right	= long(fX + fSizeX / 2.f);
	m_rcCustomColor[0].bottom = long(fY + fSizeY / 2.f);

	fX = m_fX;
	m_rcCustomColor[1].left	= long(fX - fSizeX / 2.f);
	m_rcCustomColor[1].top	= long(fY - fSizeY / 2.f);
	m_rcCustomColor[1].right	= long(fX + fSizeX / 2.f);
	m_rcCustomColor[1].bottom = long(fY + fSizeY / 2.f);

	fX = m_fX + 77.f;

	m_rcCustomColor[2].left	= long(fX - fSizeX / 2.f);
	m_rcCustomColor[2].top	= long(fY - fSizeY / 2.f);
	m_rcCustomColor[2].right	= long(fX + fSizeX / 2.f);
	m_rcCustomColor[2].bottom = long(fY + fSizeY / 2.f);

	fSizeX = 215.f;
	fSizeY = 215.f;
	fY = m_fY + 42.f;

	m_rcColor.left		= long(m_fX - fSizeX / 2.f);
	m_rcColor.top		= long(fY - fSizeY / 2.f);
	m_rcColor.right		= long(m_fX + fSizeX / 2.f);
	m_rcColor.bottom	= long(fY + fSizeY / 2.f);

	fSizeX = 67.f;
	fSizeY = 13.f;
	fX = m_fX;
	fY = m_fY + 173.f;

	m_rcButton[0].left		= long(fX - fSizeX / 2.f);
	m_rcButton[0].top		= long(fY - fSizeY / 2.f);
	m_rcButton[0].right		= long(fX + fSizeX / 2.f);
	m_rcButton[0].bottom	= long(fY + fSizeY / 2.f);

	fX = m_fX + 77.f;
	m_rcButton[1].left		= long(fX - fSizeX / 2.f);
	m_rcButton[1].top		= long(fY - fSizeY / 2.f);
	m_rcButton[1].right		= long(fX + fSizeX / 2.f);
	m_rcButton[1].bottom	= long(fY + fSizeY / 2.f);

	m_hDc = GetDC(g_hWnd);

	m_vCustomColor = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f);

	m_iPartIndex = 0;
	m_iColorIndex = 0;

	m_bPush = false;

	return S_OK;
}

int CCustomColor_UI::Update(void)
{
	KeyCheck();

	if(g_bCustomColor == false)
	{
		return 0;
	}

	RECT rcMain = { long(m_fX - m_fSizeX), long(m_fY - m_fSizeY), long(m_fX + m_fSizeX), long(m_fY + m_fSizeY) };

	D3DXVECTOR3 vMouse = Engine::Get_InputMgr()->GetMousePos();
	POINT ptMouse;
 	ptMouse.x = (long)vMouse.x;
 	ptMouse.y = (long)vMouse.y;

	//GetCursorPos(&ptMouse);

	if(Engine::Get_InputMgr()->GetDIMouseState(DIM_Lbutton) & 0x80)
	{
		if(PtInRect(&rcMain, ptMouse))
		{
			for(int i = 0; i < 4; ++i)
			{
				if(PtInRect(&m_rcCustomParts[i], ptMouse))
				{
					m_iPartIndex = i;
				}
			}

			for(int i = 0; i < 3; ++i)
			{
				if(PtInRect(&m_rcCustomColor[i], ptMouse))
				{
					m_iColorIndex = i;
				}
			}

			if(PtInRect(&m_rcColor, ptMouse))
			{
				
				D3DXVECTOR4 vColor = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
				DWORD dwColor = GetPixel(m_hDc, (int)ptMouse.x, (int)ptMouse.y);
				DWORD_To_RGBA(dwColor, vColor);

				m_vCustomColor = vColor;
				
				//¿øº»

//				IDirect3DSurface9* BackBuff;
//
//				m_pDevice->CreateOffscreenPlainSurface(WINCX, WINCY, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &BackBuff, NULL);
//
//				//HRESULT hr = m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuff);
//
//				HRESULT hr = m_pDevice->GetFrontBufferData(0, BackBuff);
//
//				if(hr != D3D_OK)
//				{
//					BackBuff->Release();
//
//					return 0;
//				}			
//
//// 				D3DLOCKED_RECT LockRect;
//// 				D3DXSaveSurfaceToFile(L"../bin/Test.png", D3DXIFF_PNG, BackBuff, NULL, NULL);
//
//				BackBuff->GetDC(&m_hDc);
//
//				DWORD dwColor = GetPixel(m_hDc, (int)ptMouse.x, (int)ptMouse.y);
//
//				D3DXVECTOR4 vColor = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
//
//// 				LPDIRECT3DTEXTURE9 pTexture = NULL;
//// 				D3DSURFACE_DESC	dsc;
//// 				
//// 				D3DXCreateTextureFromFile(m_pDevice, L"../bin/Test.png", (LPDIRECT3DTEXTURE9*)&pTexture);
//// 
//// 				int  Pitch = 0;
//// 				int	 nByte = 0;
//// 
//// 				pTexture->GetLevelDesc(0, &dsc);
//// 
//// 				int iIndex = ptMouse.y * WINCX + ptMouse.x;
//// 
//// 				pTexture->LockRect(0, &LockRect, 0, 0);
//// 
//// 				Pitch = LockRect.Pitch;
//// 				nByte = Pitch / dsc.Width;
//// 
//// 				D3DXVECTOR4 vColor = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
//// 
//// 				if(nByte == 4)
//// 				{
//// 					DWORD* pARGB = (DWORD*)LockRect.pBits;	
//// 
//// 					DWORD_To_RGBA(pARGB[iIndex], vColor);
//// 				}
//// 				else if(nByte == 2)
//// 				{
//// 					WORD* pARGB = (WORD*)LockRect.pBits;
//// 				}
//// 				else if(nByte == 1)
//// 				{
//// 					BYTE* pARGB = (BYTE*)LockRect.pBits;
//// 				}
//
//				//DWORD* pColor = (DWORD*)LockRect.pBits;
//
//				//D3DXCOLOR Color = pColor[iIndex];
//				
//				//DWORD dwColor = GetPixel(m_hDc, (int)ptMouse.x, (int)ptMouse.y);
//				
//
//				DWORD_To_RGBA(dwColor, vColor);
//
//				m_vCustomColor = vColor;
//
//// 				m_vCustomColor.x = Color.r;
//// 				m_vCustomColor.y = Color.g;
//// 				m_vCustomColor.z = Color.b;
//// 				m_vCustomColor.w = Color.a;
//
//				//pTexture->UnlockRect(0);
//
//				BackBuff->Release();
				//pTexture->Release();
			}

			for(int i = 0; i < 2; ++i)
			{
				if(PtInRect(&m_rcButton[i], ptMouse))
				{
					if(i == 0)
					{
						Engine::CGameObject* pGameObject = Engine::Get_Management()->GetGameObject(Layer_GameLogic, L"Player");

						((CPlayer*)pGameObject)->SetCustomColor(m_vCustomColor, (eCustomParts)m_iPartIndex, (eCustomColor)m_iColorIndex);						
					}
					else
					{
						g_bCustomColor = false;
					}
				}
			}
		}
	}
	
	D3DXMatrixOrthoLH(&m_matProj, (FLOAT)WINCX, (FLOAT)WINCY, 0.f, 1.f);
	m_matView._11 = m_fSizeX;
	m_matView._22 = m_fSizeY;
	m_matView._33 = 1.f;

	m_matView._41 = m_fX - (WINCX >> 1);
	m_matView._42 = -m_fY + (WINCY >> 1);

	Engine::CGameObject::Update();

	return 0;
}

void CCustomColor_UI::Render(void)
{
	if(g_bCustomColor == false)
	{
		return;
	}

	D3DXMATRIX matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

HRESULT CCustomColor_UI::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResources(Resource_Static, L"CustomColor_UI");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(map<TCHAR*, Engine::CComponent*>::value_type(L"Texture", pComponent));

	return S_OK;
}

void CCustomColor_UI::DWORD_To_RGBA(DWORD color, D3DXVECTOR4& vColor)
{
	DWORD r,g,b,a;

	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);
	a = 255;

	vColor.x = r / 255.f;
	vColor.y = g / 255.f;
	vColor.z = b / 255.f;
	vColor.w = a / 255.f;
}

void CCustomColor_UI::KeyCheck(void)
{
	if(Engine::Get_InputMgr()->GetDIKeyState(DIK_P) & 0x80)
	{
		if(m_bPush)
		{
			return;
		}

		m_bPush = true;

		if(g_bCustomColor)
		{
			g_bCustomColor = false;
		}
		else
			g_bCustomColor = true;
	}
	else
		m_bPush = false;
}