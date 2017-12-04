/*!
 * \file StaticMesh.h
 * \date 2016/07/10 21:59
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

#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
#ifdef _MY_TOOL
public:
#else
private:
#endif
	LPDIRECT3DTEXTURE9* m_ppTexture;
	LPDIRECT3DTEXTURE9* m_ppNormalTexture;
	LPDIRECT3DTEXTURE9* m_ppSpecularTexture;
	D3DMATERIAL9*		m_pMtrl;
	LPD3DXMESH			m_pMesh;
	LPD3DXBUFFER		m_pSubset;
	DWORD				m_dwSubsetCnt;

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pFileName);

public:
	virtual HRESULT		LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void		RenderMesh(LPD3DXEFFECT pEffect, const D3DXMATRIX* pMatWorld);
	virtual CResources* CloneResource(void);
	virtual DWORD		Release(void);

public:
	LPD3DXMESH			GetMesh(void);
	LPD3DXBUFFER		GetSubSet(void);

private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaticMesh(const CStaticMesh& rhs);

public:
	virtual ~CStaticMesh(void);
};

END

#endif // StaticMesh_h__