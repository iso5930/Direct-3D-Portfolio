/*!
 * \file Renderer.h
 * \date 2016/07/01 22:42
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

#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CScene;
class CFont;
class CGameObject;
class CShader;
class CRcTex;

class ENGINE_DLL CRenderer
{
private:
	list<CGameObject*>	m_RenderGroup[Type_End];

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	CScene*				m_pScene;
	float				m_fTime;
	TCHAR				m_szFps[128];
	DWORD				m_dwCount;
	CFont*				m_pFont;

private:
	//Blur
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DSURFACE9 m_pSurface;
	LPDIRECT3DSURFACE9 m_pDepthStencil;

	LPDIRECT3DSURFACE9 m_pBackSurface;
	LPDIRECT3DSURFACE9 m_pBackDepthStencil;

	CShader* m_pBlurShader;
	CRcTex* m_pBuffer;
	bool m_bCreate;

public:
	bool m_bBlur;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Init_Scene(void);
	
public:
	void Add_RenderGroup(CGameObject* pObj, eRenderType _Type);
	void Init_Blur();

public:
	void Render(const float& fTime);
	void Release(void);

public:
	void SetScene(CScene* pScene);

private:
	void Render_FPS(const float& fTime);
	void Render_Shader(void);
	void Render_UI(void);
	void Render_Skill(void);
	void Render_Priority(void);

private:
	void Blur_Begin();
	void Blur_End();

private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CRenderer(void);
};

END

#endif // Renderer_h__