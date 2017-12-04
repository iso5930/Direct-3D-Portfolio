/*!
 * \file DynamicMesh.h
 * \date 2016/07/10 22:01
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

#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CLoader;
class CAniCtrl;

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	CLoader*		m_pLoader;
	CAniCtrl*		m_pAniCtrl;
	LPD3DXFRAME		m_pRootBoon;
	eCollisionType	m_eType;

private:
	LPDIRECT3DTEXTURE9	m_pHWSkinningTexture;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName);

public:
	virtual CResources* CloneResource(void);
	virtual HRESULT		LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void		RenderMesh(LPD3DXEFFECT pEffect, const D3DXMATRIX* pMatWorld);
	virtual DWORD		Release(void);

private:
	void UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	void SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame);
	void Find_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_FRAME* pFrame);
	void Render_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_MESHCONTAINER* pMeshContainer);

private:
	void Render_ShadowMeshContainer(LPD3DXEFFECT pEffect, DERIVED_MESHCONTAINER* pMeshContainer);
	void Find_ShadowMeshContainer(LPD3DXEFFECT pEffect, DERIVED_FRAME* pFrame);

public:
	void FrameMove(const float& fTimePerSec);
	void SetAnimation(UINT iAniIdx);
	void SetMagicNum(DOUBLE dMagicNum);
	void SetCustomRGB(D3DXVECTOR4 vCustomColor, eCustomColor _eColor, eCustomParts _Parts);
	void Find_Mesh(DERIVED_FRAME* pFrame, D3DXVECTOR4 vCustomColor, eCustomColor _eColor, eCustomParts _Parts);
	void SetCustomColor(DERIVED_MESHCONTAINER* pFrame, D3DXVECTOR4 vCustomColor, eCustomColor _eColor, eCustomParts _Parts);
	void Render_Shadow(LPD3DXEFFECT pEffect);

public:
	const D3DXMATRIX*	FindFrame(const char* pFrameName); //무기 붙일때 사용하는 것 같음..
	DOUBLE				GetTrackPosition(void);
	DOUBLE				GetPeriod(void);
	void				SetTrackPos(DOUBLE dTrackPosition);
	void				SetOwner(eCollisionType _Type);
	
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CDynamicMesh(CONST CDynamicMesh& rhs);

public:
	virtual ~CDynamicMesh(void);
};

END

#endif // DynamicMesh_h__