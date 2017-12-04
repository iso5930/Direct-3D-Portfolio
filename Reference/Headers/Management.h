/*!
 * \file Management.h
 * \date 2016/06/29 11:17
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CComponent;

class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	LPDIRECT3DDEVICE9	m_pDevice;
public:
	CRenderer*			m_pRenderer;
private:
	CScene*				m_pScene;

public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void	Update(void);
	void	Render(const float& fTime);
	void	Add_RenderGroup(eRenderType _Type, CGameObject* pObj);
	void	RenderGroupRelease(void);

public:
	CComponent*		GetComponent(WORD LayerID, TCHAR* pObjKey, TCHAR* pComponentKey);
	CGameObject*	GetGameObject(WORD LayerID, TCHAR* pObjKey);

private:
	void	Release(void);

public:
	template<typename T>
	HRESULT SceneChange(T& Functor);
	
private:
	CManagement(void);
	~CManagement(void);
};

template<typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
	{
		Engine::Safe_Delete(m_pScene);
	}
	
	if(FAILED(Functor(&m_pScene, m_pDevice)))
	{
		return E_FAIL;
	}

	//m_pRenderer->Release();
	m_pRenderer->SetScene(m_pScene);
		
	return S_OK;
}

END

#endif // Management_h__