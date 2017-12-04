/*!
 * \file Stage.h
 * \date 2016/07/16 22:21
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

#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

class CStage : public Engine::CScene
{
public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Initialize(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Light(void);
	void	Release(void);

private:
	CStage(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStage(void);
};

#endif // Stage_h__