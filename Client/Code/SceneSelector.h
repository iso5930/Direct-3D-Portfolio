/*!
 * \file SceneSelector.h
 * \date 2016/07/06 21:32
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

#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Logo.h"
#include "Stage.h"

class CSceneSelector
{
private:
	eSceneID m_eSceneID;

public:
	HRESULT operator () (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice)
	{
		switch(m_eSceneID)
		{
		case SceneID_Logo:
			*ppScene = CLogo::Create(pDevice);
			break;

		case SceneID_Stage:
			*ppScene = CStage::Create(pDevice);
			break;
		}

		if(*ppScene == NULL)
		{
			return E_FAIL;
		}

		return S_OK;
	}

public:
	explicit CSceneSelector(eSceneID _SceneID)
		:m_eSceneID(_SceneID){}
	~CSceneSelector(void){}
};

#endif // SceneSelector_h__