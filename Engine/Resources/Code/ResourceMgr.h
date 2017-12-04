/*!
 * \file ResourceMgr.h
 * \date 2016/06/29 11:18
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

#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CResources;

class ENGINE_DLL CResourcsMgr
{
	DECLARE_SINGLETON(CResourcsMgr);
private:
	map<const TCHAR*, CResources*>*	m_pmapResource;
	WORD							m_wReservedSize;

public:
	HRESULT	ReserveContainerSize(WORD wSize);
	HRESULT AddBuffer(LPDIRECT3DDEVICE9 pDevice, WORD wContainerIdx, eBuffer_Type _Type, const TCHAR* pResourceKey);
	HRESULT AddTexture(LPDIRECT3DDEVICE9 pDevice, WORD wContainerIdx, eTexture_Type _Type, const TCHAR* pResourceKey, const TCHAR* pFilePath, WORD wCnt);
	HRESULT AddMesh(LPDIRECT3DDEVICE9 pDevice, WORD wConrainerIdx, eMeshType _Type, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName);
	HRESULT	Add2DSprite(LPDIRECT3DDEVICE9 pDevice, WORD wConrainerIdx, const TCHAR* pResourceKey, const TCHAR* pFilePath, WORD wCnt);

public:
	CResources* FindResources(WORD wContainerIdx, const TCHAR* pResourceKey);
	CResources* CloneResources(WORD wContainerIdx, const TCHAR* pResourceKey);

public:
	void ResourceReset(WORD wContainerIdx);

private:
	void Release(void);

private:
	CResourcsMgr(void);
	~CResourcsMgr(void);
};

END

#endif // ResourceMgr_h__