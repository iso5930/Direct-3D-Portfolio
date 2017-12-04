/*!
 * \file DynamicCamera.h
 * \date 2016/07/08 13:47
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

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera : public Engine::CCamera
{
private:
	D3DXVECTOR3*	m_pTarget;
	float			m_fCamSpeed;
	float			m_fDist;
	bool			m_bMouse;
	bool			m_bPush;

private:
	const D3DXMATRIX*	m_pmatInfo;
	const D3DXMATRIX*	m_pmatCombined;
	bool				m_bDarkKnight;

private:
	D3DXMATRIX			m_matPrevView;
	D3DXVECTOR3			m_vPrevEye;

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pTarget);

public:
	virtual int Update(void);

private:
	HRESULT Initialize(D3DXVECTOR3* pTarget);
	void	KeyCheck(void);
	void	FixMouse(void);
	void	MouseMove(void);

public:
	void SetTarget(D3DXVECTOR3* pPos);
	D3DXVECTOR3 GetRight(void);

public:
	void	SetDarkKnight(bool _bTrans);
	void	SetMatrix(const D3DXMATRIX* pMatInfo, const D3DXMATRIX* pmatCombined);
	
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDynamicCamera(void);
};
#endif // DynamicCamera_h__