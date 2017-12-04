#include "ResourceMgr.h"
#include "Resources.h"
#include "RcTex.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "CubeTex.h"
#include "2DSprite.h"

IMPLEMENT_SINGLETON(Engine::CResourcsMgr);

Engine::CResourcsMgr::CResourcsMgr(void)
:m_pmapResource(NULL), m_wReservedSize(0)
{

}

Engine::CResourcsMgr::~CResourcsMgr(void)
{
	Release();
}

HRESULT Engine::CResourcsMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice, WORD wContainerIdx, eBuffer_Type _Type, const TCHAR* pResourceKey)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = FindResources(wContainerIdx, pResourceKey);

	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"버퍼가 이미 추가 되어 있음");
		return E_FAIL;
	}

	CVIBuffer*		pBuffer = NULL;

	switch(_Type)
	{
	case Buffer_RcTex:
		pBuffer = CRcTex::Create(pDevice);
		break;

	case Buffer_CubeTex:
		pBuffer = CCubeTex::Create(pDevice);
		break;
	}

	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pmapResource[wContainerIdx].insert(map<const TCHAR*, CResources*>::value_type(pResourceKey, pBuffer));

	return S_OK;
}

HRESULT Engine::CResourcsMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice, WORD wContainerIdx, eTexture_Type _Type, const TCHAR* pResourceKey, const TCHAR* pFilePath, WORD wCnt)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = FindResources(wContainerIdx, pResourceKey);
	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"텍스쳐가 이미 추가 되어 있음");
		return E_FAIL;
	}

	pResource = CTexture::Create(pDevice, _Type, pFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, L"리소스 할당 실패");

	m_pmapResource[wContainerIdx].insert(map<const TCHAR*, CResources*>::value_type(pResourceKey, pResource));
	
	return S_OK;
}

HRESULT Engine::CResourcsMgr::AddMesh(LPDIRECT3DDEVICE9 pDevice, WORD wConrainerIdx, eMeshType _Type, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName)
{
	CResources* pResource = FindResources(wConrainerIdx, pMeshKey);

	if(pResource != NULL)
	{
		return E_FAIL;
	}

	switch(_Type)
	{
	case Mesh_Static:
		pResource = CStaticMesh::Create(pDevice, pFilePath, pFileName);
		break;

	case Mesh_Dynamic:
		pResource = CDynamicMesh::Create(pDevice, pFilePath, pFileName);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_pmapResource[wConrainerIdx].insert(map<const TCHAR*, CResources*>::value_type(pMeshKey, pResource));
	
	return S_OK;
}

Engine::CResources* Engine::CResourcsMgr::FindResources(WORD wContainerIdx, const TCHAR* pResourceKey)
{
	map<const TCHAR*, CResources*>::iterator iter = m_pmapResource[wContainerIdx].begin();
	map<const TCHAR*, CResources*>::iterator iter_end = m_pmapResource[wContainerIdx].end();

	map<const TCHAR*, CResources*>::iterator	Find_iter = find_if(iter, iter_end, CTagFinder(pResourceKey));

	if(Find_iter == iter_end)
		return NULL;

	return Find_iter->second;
}

Engine::CResources* Engine::CResourcsMgr::CloneResources(WORD wContainerIdx, const TCHAR* pResourceKey)
{
	CResources*		pResource = FindResources(wContainerIdx, pResourceKey);
	NULL_CHECK_RETURN(pResource, NULL);

	return pResource->CloneResource();
}

void Engine::CResourcsMgr::ResourceReset(WORD wContainerIdx)
{
	map<const TCHAR*, CResources*>::iterator iter = m_pmapResource[wContainerIdx].begin();
	map<const TCHAR*, CResources*>::iterator iter_end = m_pmapResource[wContainerIdx].end();

	for_each(iter, iter_end, CDeleteMap());

	m_pmapResource[wContainerIdx].clear();
}

void Engine::CResourcsMgr::Release(void)
{
	if(0 == m_wReservedSize)
		return;

	for (size_t i = 0; i < m_wReservedSize; ++i)
	{
		map<const TCHAR*, CResources*>::iterator	iter = m_pmapResource[i].begin();
		map<const TCHAR*, CResources*>::iterator	iter_end = m_pmapResource[i].end();

		DWORD		dwRefCnt = 0;

		for(iter; iter != iter_end; ++iter)
		{
			dwRefCnt = iter->second->Release();

			if(dwRefCnt == 0)
				Safe_Delete(iter->second);
		}
		m_pmapResource[i].clear();
	}	
	Engine::Safe_Delete_Array(m_pmapResource);
}

HRESULT Engine::CResourcsMgr::ReserveContainerSize(WORD wSize)
{
	if(m_wReservedSize != 0	|| m_pmapResource != NULL)
		return E_FAIL;

	m_pmapResource = new map<const TCHAR*, CResources*>[wSize];

	m_wReservedSize = wSize;

	return S_OK;
}

HRESULT Engine::CResourcsMgr::Add2DSprite(LPDIRECT3DDEVICE9 pDevice, WORD wConrainerIdx, const TCHAR* pResourceKey, const TCHAR* pFilePath, WORD wCnt)
{	
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = FindResources(wConrainerIdx, pResourceKey);

	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"텍스쳐가 이미 추가 되어 있음");
		return E_FAIL;
	}

	pResource = C2DSprite::Create(pDevice, pFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, L"리소스 할당 실패");

	m_pmapResource[wConrainerIdx].insert(map<const TCHAR*, CResources*>::value_type(pResourceKey, pResource));

	return S_OK;
}