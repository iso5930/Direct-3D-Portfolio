/*!
 * \file DefaultEffect.h
 * \date 2016/08/16 11:16
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

#ifndef DefaultEffect_h__
#define DefaultEffect_h__

#include "Effect.h"

class CDefaultEffect : public CEffect
{
private:
	bool	m_bAxis;
	float	m_fTime;
	float	m_fAccTime;
	float	m_fAngleSpeed;
	float	m_fAngle;
	float	m_fDirSpeed;
	float	m_fScaleSpeed;
	eAngle	m_AngleType;

private:
	D3DXVECTOR3		m_vScale;
	D3DXMATRIX		m_matBill;
	TCHAR			m_szTextureName[128];
	int				m_iTextureIndex;

public:
	static CDefaultEffect*	Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 vPos, D3DXVECTOR3 vStartScale, D3DXVECTOR3 vDir, float fTime, float fDirSpeed, float fAngleSpeed, float fStartAngle, float fAngle, eAngle _Angle, bool bAxis, D3DXVECTOR3 vScale, float fScaleSpeed, PTCHAR pName, int iTexIndex = 0);

private:
	virtual HRESULT Initialize(void);

public:
	virtual int		Update(void);
	virtual void	Render(void);

private:
	HRESULT		InitData(D3DXVECTOR3 vPos, D3DXVECTOR3 vScale, float fTime, D3DXVECTOR3 vDir);
	HRESULT		InitData2(D3DXVECTOR3 vScale, float fDirSpeed, float fAngelSpeed, float fStartAngle, eAngle _Angle, bool bAxis, float fScaleSpeed);
	HRESULT		AddComponent(void);

public:
	void		SetSpeed(float _AngleSpeed, float _Angle, float _Speed);
	void		SetTex(int iIndex, PTCHAR pName);
	void		SetAngel(float fAngle);

private:
	explicit CDefaultEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDefaultEffect(void);
};

#endif // DefaultEffect_h__