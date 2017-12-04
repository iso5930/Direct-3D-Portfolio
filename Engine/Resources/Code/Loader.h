/*!
 * \file Loader.h
 * \date 2016/07/11 20:35
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

#ifndef Loader_h__
#define Loader_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLoader : public ID3DXAllocateHierarchy
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	const TCHAR*		m_pPath;

private:
	void AllocateName(const CHAR* const pName, CHAR** ppNewName);
	bool Find_Name(TCHAR* pFindName, TCHAR* pFileName);

public:
	virtual STDMETHODIMP CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	virtual STDMETHODIMP CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	virtual STDMETHODIMP DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree);
	virtual STDMETHODIMP DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

public:
	explicit CLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath);
	~CLoader(void);
};

END

#endif // Loader_h__