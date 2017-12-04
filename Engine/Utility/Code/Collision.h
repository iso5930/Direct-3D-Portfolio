/*!
 * \file Collision.h
 * \date 2016/08/11 18:31
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

#ifndef Collision_h__
#define Collision_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CTransform;
class CVIBuffer;

class ENGINE_DLL CCollision
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	CTransform*			m_pInfo;
	LPD3DXMESH			m_pMesh;
	CVIBuffer*			m_pBuffer;
	OBB					m_OBB;
	D3DXMATRIX			m_matWorld;
	float				m_fRadius;

private:
	const D3DXMATRIX*	m_pmatInfo;
	const D3DXMATRIX*	m_pmatCombined;

private:
	bool				m_bOBB;
	bool				m_bSphere;
				
public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pDevice, float fRadius, const D3DXMATRIX* pmatInfo, const D3DXMATRIX* pmatCombined);

private:
	HRESULT Initialize(float fRadius, const D3DXMATRIX* pmatInfo, const D3DXMATRIX* pmatCombined);
	void	Release(void);
	void	InitPoint(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);
	void	InitAxis(OBB* pOBB);

public:
	int		Update(void);
	void	Render(void);

public:
	void		SetPos(D3DXVECTOR3 vPos);
	void		SetScale(D3DXVECTOR3 vScale);
	void		SetBOOL(bool _Obb, bool _Sphere);

public:
	D3DXMATRIX*	GetWorldMatrix(void);
	OBB*		GetOBB(void);
	D3DXVECTOR3	GetPos(void);
	float		GetRadius(void);
	void		SetCombineMatrix(const D3DXMATRIX* pMatCombine);
	
private:
	explicit CCollision(LPDIRECT3DDEVICE9 pDevice);

public:
	~CCollision(void);
};

END

#endif // Collision_h__