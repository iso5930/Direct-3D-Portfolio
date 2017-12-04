#include "ShadowMgr.h"
#include "Export_Function.h"
#include "Component.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(Engine::CShadowMgr)

Engine::CShadowMgr::CShadowMgr(void)
:m_pDevice(Engine::Get_GraphicDev()->GetDevice()), m_pHWBackBuffuer(NULL), m_pHWDepthStensilBuffer(NULL), m_pShadowDepthStencil(NULL), m_pShadowRenderTarget(NULL), m_pShader(NULL)
{

}

Engine::CShadowMgr::~CShadowMgr(void)
{
	Release();
}

HRESULT Engine::CShadowMgr::InitShadowMgr(void)
{
	HRESULT hr = NULL;

	hr = m_pDevice->CreateTexture(2048, 2048, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pDevice->CreateDepthStencilSurface(2048, 2048, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	Engine::CComponent* pComponent = NULL;

	//Shader
	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Shadow");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	//Init
	D3DXMatrixIdentity(&m_matLightView);
	D3DXMatrixIdentity(&m_matLightProj);
	D3DXMatrixIdentity(&m_matLightViewProj);
	
	return S_OK;
}

void Engine::CShadowMgr::ShadowBegin(void)
{
	m_pHWBackBuffuer = NULL;
	m_pHWDepthStensilBuffer = NULL;
	m_pDevice->GetRenderTarget(0, &m_pHWBackBuffuer);
	m_pDevice->GetDepthStencilSurface(&m_pHWDepthStensilBuffer);

	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if( SUCCEEDED( m_pShadowRenderTarget->GetSurfaceLevel( 0, &pShadowSurface ) ) )
	{
		m_pDevice->SetRenderTarget( 0, pShadowSurface );
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	m_pDevice->SetDepthStencilSurface( m_pShadowDepthStencil );
	m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0 );
}

void Engine::CShadowMgr::ShadowEnd(void)
{
	m_pDevice->SetRenderTarget( 0, m_pHWBackBuffuer );
	m_pDevice->SetDepthStencilSurface(m_pHWDepthStensilBuffer);

	m_pHWBackBuffuer->Release();
	m_pHWBackBuffuer = NULL;
	m_pHWDepthStensilBuffer->Release();
	m_pHWDepthStensilBuffer = NULL;
}

void Engine::CShadowMgr::ShadowSetConstantTable(D3DXMATRIX* pMatWorld, D3DXVECTOR3 vLightPos)
{
	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();

	D3DXVECTOR3 vPos(pMatWorld->_41, pMatWorld->_42, pMatWorld->_43); //플레이어 Pos;
	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&vDir, &D3DXVECTOR3(50.f, 200.f, 0.f));
	
	D3DXVECTOR3 vShadowPos = vPos + (vDir) * 300.f;
	D3DXMatrixLookAtLH(&m_matLightView, &vShadowPos, &vPos, &D3DXVECTOR3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matLightProj, D3DXToRadian(45.f), (float)1920 / (float)1080, 1.f, 1000.f);

	m_matLightViewProj = m_matLightView * m_matLightProj;
	
	pEffect->SetMatrix("g_matWorld", pMatWorld);
	pEffect->SetMatrix("g_matLightViewProj", &m_matLightViewProj);
}

void Engine::CShadowMgr::Release(void)
{	
	Safe_Release(m_pShadowRenderTarget);
	Safe_Release(m_pShadowDepthStencil);
}

LPDIRECT3DTEXTURE9 Engine::CShadowMgr::GetRenderTarger(void)
{	
	return m_pShadowRenderTarget;
}

LPD3DXEFFECT Engine::CShadowMgr::GetEffect(void)
{
	return m_pShader->GetEffectHandle();
}

void Engine::CShadowMgr::SetShadowEffect(LPD3DXEFFECT pEffect, D3DXVECTOR3 vLightPos)
{
	//맵의 이펙트에 쉐이더를 넣어주기 위해서 만든 함수.

	pEffect->SetTexture("g_ShadowMap_Tex", m_pShadowRenderTarget);
	pEffect->SetMatrix("g_matLightViewProj", &m_matLightViewProj);
}