/*!
 * \file Camera_Tool.h
 * \date 2016/07/27 19:11
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

#ifndef Camera_Tool_h__
#define Camera_Tool_h__

#include "Camera.h"

class CCamera_Tool : public Engine::CCamera
{
private:
	float	m_fSpeed;

public:
	virtual int Update(void);

public:
	HRESULT Initialize(D3DXVECTOR3 vEye, D3DXVECTOR3 vAt);
	void	KeyCheck(void);
		
public:
	explicit CCamera_Tool(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCamera_Tool(void);
};

#endif // Camera_Tool_h__