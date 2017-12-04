#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice), m_pInfo(NULL), m_pTexture(NULL), m_pBuffer(NULL)
{
	
}

CEffect::~CEffect(void)
{

}