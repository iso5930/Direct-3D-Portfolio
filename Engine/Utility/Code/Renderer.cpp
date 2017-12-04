#include "Renderer.h"
#include "Scene.h"
#include "Font.h"
#include "Export_Function.h"
#include "GameObject.h"
#include "Shader.h"
#include "RcTex.h"

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice), m_fTime(0.f), m_dwCount(0), m_pScene(NULL), m_pFont(NULL)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);

	m_bCreate = false;
	m_bBlur = false;
}

Engine::CRenderer::~CRenderer(void)
{
	m_pBuffer->Release();
	m_pBlurShader->Release();
}

Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer* pRenderer = new CRenderer(pDevice);

	if(FAILED(pRenderer->Init_Scene()))
	{
		Safe_Delete(pRenderer);
	}

	return pRenderer;
}

HRESULT Engine::CRenderer::Init_Scene(void)
{
	m_pFont = Engine::Get_FontMgr()->GetFont(L"궁서");
	
	return S_OK;
}

void Engine::CRenderer::Render(const float& fTime)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	if(m_bCreate == true && m_bBlur == true && m_pScene != NULL)
	{
		Blur_Begin();
	}

	Render_Priority();
	Render_Shader();
	Render_Skill();

	if(m_pScene != NULL)
	{
		m_pScene->Render();
	}

	Render_UI();

	if(m_bCreate == true && m_bBlur == true && m_pScene != NULL)
	{
		Blur_End();
	}

	Render_FPS(fTime);
	
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void Engine::CRenderer::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}

void Engine::CRenderer::Add_RenderGroup(CGameObject* pObj, eRenderType _Type)
{
	m_RenderGroup[_Type].push_back(pObj);
}

void Engine::CRenderer::Render_FPS(const float& fTime)
{
	m_fTime += fTime;
	++m_dwCount;

	if(m_fTime >= 1.f)
	{
		wsprintf(m_szFps, L"Fps :  %d", m_dwCount);
		m_fTime = 0.f;
		m_dwCount = 0;
	}

	m_pFont->Render(m_szFps, D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR3(0.f, 0.f, 0.f));
}

void Engine::CRenderer::Render_Shader(void)
{
	list<CGameObject*>::iterator iter = m_RenderGroup[Type_Shader].begin();
	list<CGameObject*>::iterator iter_end = m_RenderGroup[Type_Shader].end();
	
	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Render_UI(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	list<CGameObject*>::iterator iter = m_RenderGroup[Type_UI].begin();
	list<CGameObject*>::iterator iter_end = m_RenderGroup[Type_UI].end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Release(void)
{
	for(int i = 0; i < Type_End; ++i)
	{
		m_RenderGroup[i].clear();
	}
}

void Engine::CRenderer::Render_Skill(void)
{
	Engine::Get_SkillMgr()->Render();
}

void Engine::CRenderer::Render_Priority(void)
{
	list<CGameObject*>::iterator iter = m_RenderGroup[Type_Priority].begin();
	list<CGameObject*>::iterator iter_end = m_RenderGroup[Type_Priority].end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Blur_Begin()
{
	m_pDevice->GetRenderTarget(0, &m_pBackSurface);
	m_pDevice->GetDepthStencilSurface(&m_pBackDepthStencil);

	m_pDevice->SetRenderTarget(0, m_pSurface);
	m_pDevice->SetDepthStencilSurface(m_pDepthStencil);
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.0f, 0);

	m_pDevice->SetRenderTarget(0, m_pBackSurface);
	m_pDevice->SetDepthStencilSurface(m_pBackDepthStencil);

	if(m_pBackSurface != NULL)
	{
		m_pBackSurface->Release();
		m_pBackSurface = NULL;
	}

	if(m_pBackDepthStencil != NULL)
	{
		m_pBackDepthStencil->Release();
		m_pBackDepthStencil = NULL;
	}
	//Clear

	m_pDevice->GetRenderTarget(0, &m_pBackSurface);
	m_pDevice->GetDepthStencilSurface(&m_pBackDepthStencil);

	m_pDevice->SetRenderTarget(0, m_pSurface);
	m_pDevice->SetDepthStencilSurface(m_pDepthStencil);
}

void Engine::CRenderer::Blur_End()
{
	m_pDevice->SetRenderTarget(0, m_pBackSurface);
	m_pDevice->SetDepthStencilSurface(m_pBackDepthStencil);

	if(m_pBackSurface != NULL)
	{
		m_pBackSurface->Release();
		m_pBackSurface = NULL;
	}

	if(m_pBackDepthStencil != NULL)
	{
		m_pBackDepthStencil->Release();
		m_pBackDepthStencil = NULL;
	}

	LPD3DXEFFECT pEffect = m_pBlurShader->GetEffectHandle();
	FAILED_CHECK_RETURN(pEffect, );

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	pEffect->SetFloat("g_fBlurWidth", 1.f);
	pEffect->SetTexture("g_SceneTexture", m_pTexture);
	pEffect->CommitChanges();

	m_pBuffer->Render(NULL);

	pEffect->EndPass();
	pEffect->End();
}

void Engine::CRenderer::Init_Blur()
{
	//블러를 위해서 만들어 주자...
 	m_pDevice->CreateTexture(1900, 1060, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture, NULL);
 	m_pTexture->GetSurfaceLevel(0, &m_pSurface);
 	m_pDevice->CreateDepthStencilSurface(1900, 1060, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pDepthStencil, NULL);
 	
 	m_pBackSurface = NULL;
 	m_pBackDepthStencil = NULL;
 
 	m_pBlurShader = (Engine::CShader*)Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Blur");
 
 	m_pBuffer = (Engine::CRcTex*)Get_ResourceMgr()->CloneResources(Resource_Static, L"Buffer_RcTex");

	m_bCreate = true;
}