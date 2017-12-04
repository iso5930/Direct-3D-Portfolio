/*!
 * \file ShadowMgr.h
 * \date 2016/08/25 1:10
 *
 * \author Jin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ShadowMgr_h__
#define ShadowMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CShadowMgr
{
public:
	DECLARE_SINGLETON(CShadowMgr);

private:
	CShader*			m_pShader;
	LPDIRECT3DDEVICE9	m_pDevice;

private:
	D3DXMATRIX m_matLightView;
	D3DXMATRIX m_matLightProj;
	D3DXMATRIX m_matLightViewProj;

private:
	LPDIRECT3DSURFACE9		m_pHWBackBuffuer;
	LPDIRECT3DSURFACE9		m_pHWDepthStensilBuffer;
	//지역변수

private:
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;

public:
	HRESULT	InitShadowMgr(void);

public:
	void	ShadowBegin(void);
	void	ShadowEnd(void);
	void	ShadowSetConstantTable(D3DXMATRIX* pMatWorld, D3DXVECTOR3 vLightPos);

public:
	LPDIRECT3DTEXTURE9	GetRenderTarger(void);
	LPD3DXEFFECT		GetEffect(void);

public:
	void	SetShadowEffect(LPD3DXEFFECT pEffect, D3DXVECTOR3 vLightPos);

private:
	void	Release(void);

private:
	CShadowMgr(void);
	~CShadowMgr(void);
};

END

#endif // ShadowMgr_h__