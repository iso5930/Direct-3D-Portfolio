/*!
 * \file Mesh.h
 * \date 2016/07/07 21:17
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

#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh : public CResources
{
public:
	virtual HRESULT		LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)PURE;
	virtual void		RenderMesh(LPD3DXEFFECT pEffect, const D3DXMATRIX* pMatWorld)PURE;
	virtual CResources* CloneResource(void)PURE;

protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& rhs);

public:
	virtual ~CMesh(void);
};

END

#endif // Mesh_h__