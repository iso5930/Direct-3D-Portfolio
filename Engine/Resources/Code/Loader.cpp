#include "Loader.h"

Engine::CLoader::CLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath)
: m_pDevice(pDevice), m_pPath(pPath)
{

}

Engine::CLoader::~CLoader(void)
{

}

STDMETHODIMP Engine::CLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	DERIVED_FRAME* pNewFrame = new DERIVED_FRAME;
	ZeroMemory(pNewFrame, sizeof(DERIVED_FRAME));

	if(Name != NULL)
	{
		AllocateName(Name, &pNewFrame->Name);
	}

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	pNewFrame->CombinedMatrix = pNewFrame->TransformationMatrix = matIdentity;
	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP Engine::CLoader::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	LPD3DXMESH pMesh = pMeshData->pMesh;

	if(pMesh->GetFVF() == 0)
	{
		return E_FAIL;
	}

	HRESULT hr = NULL;

	DERIVED_MESHCONTAINER* pMeshContainer = new DERIVED_MESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(DERIVED_MESHCONTAINER));

	if(Name != NULL)
	{
		AllocateName(Name, &pMeshContainer->Name);
	}

	//인접정보 셋팅
	DWORD dwNumFaces = pMesh->GetNumFaces();
	pMeshContainer->pAdjacency = new DWORD[dwNumFaces * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwNumFaces * 3);

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

// 	if(!(pMesh->GetFVF() & D3DFVF_NORMAL)) //법선 벡터가 없을때..
// 	{
// 		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, m_pDevice, &pMeshContainer->MeshData.pMesh);
// 
// 		D3DXComputeNormals(pMeshContainer->MeshData.pMesh, pMeshContainer->pAdjacency);
// 	}
// 	else
// 	{
// 		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), m_pDevice, &pMeshContainer->MeshData.pMesh);
// 
// 		pMesh->AddRef();
// 	}

	pMeshContainer->NumMaterials = max(NumMaterials, 1);
	//혹시나 재질정보가 없을수도 있기 때문에..

	pMeshContainer->pMaterials	= new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTexture	= new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);
	ZeroMemory(pMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	pMeshContainer->ppTextureNormal = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials]; //노말 맵 셋팅.
	ZeroMemory(pMeshContainer->ppTextureNormal, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	pMeshContainer->ppTextureCustom = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials]; //커스텀 맵 셋팅
	ZeroMemory(pMeshContainer->ppTextureCustom, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	pMeshContainer->ppTextureSpecular = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials]; //스펙큘러 맵 셋팅
	ZeroMemory(pMeshContainer->ppTextureSpecular, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	ZeroMemory(&pMeshContainer->vCustomColorR, sizeof(D3DXVECTOR4));
	ZeroMemory(&pMeshContainer->vCustomColorG, sizeof(D3DXVECTOR4));
	ZeroMemory(&pMeshContainer->vCustomColorB, sizeof(D3DXVECTOR4));

	if(NumMaterials > 0) //재질 정보가 있을 경우에
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);

		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			if(pMeshContainer->pMaterials[i].pTextureFilename == NULL)
			{
				continue;
			}

			TCHAR szFullPath[128] = L"";
			TCHAR szFileName[128] = L"";
						
			lstrcpy(szFullPath, m_pPath);
			MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[i].pTextureFilename, strlen(pMeshContainer->pMaterials[i].pTextureFilename), szFileName, 128);

			lstrcat(szFullPath, szFileName);

			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pMeshContainer->ppTexture[i]); //일반 텍스쳐.
			FAILED_CHECK(hr);

			TCHAR szNormalName[128] = L"";
			TCHAR szCustomName[128] = L"";
			TCHAR szSpecularName[128] = L"";

			int iSize = lstrlen(szFileName) - 4;

			for(int j = 0; j < iSize; ++j)
			{
				szNormalName[j] = szFileName[j];
				szCustomName[j] = szFileName[j];
				szSpecularName[j] = szFileName[j];
			}

			if(!(lstrcmp(&szNormalName[iSize - 1], L"A")))
				pMeshContainer->bAlpha = true;
			else
				pMeshContainer->bAlpha = false;

			lstrcat(szNormalName, L"_normal.tga");
			lstrcat(szCustomName, L"_Custom.tga");
			lstrcat(szSpecularName, L"_SP.tga");

			TCHAR szFullName[128] = L"";

			lstrcpy(szFullName, m_pPath);
			lstrcat(szFullName, szNormalName);

			if(FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullName, &pMeshContainer->ppTextureNormal[i])))
			{
				pMeshContainer->ppTextureNormal[i] = NULL;
			}

			lstrcpy(szFullName, m_pPath);
			lstrcat(szFullName, szCustomName);

			if(FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullName, &pMeshContainer->ppTextureCustom[i])))
			{
				pMeshContainer->ppTextureCustom[i] = NULL;
			}
			else
			{
				for(int i = 0; i < CustomParts_End; ++i)
				{
					TCHAR szFindName[25] = L"";

					switch(i)
					{
					case CustomParts_Weapon:
						lstrcpy(szFindName, L"staff");
						break;

					case CustomParts_Upper:
						lstrcpy(szFindName, L"upper");
						break;

					case CustomParts_Lower:
						lstrcpy(szFindName, L"lower");
						break;

					case CustomParts_Foot:
						lstrcpy(szFindName, L"foot");
						break;
					}

					if(Find_Name(szFindName, szFileName))
					{
						pMeshContainer->CustomParts = (eCustomParts)i;

						pMeshContainer->vCustomColorR = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f);
						pMeshContainer->vCustomColorG = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f);
						pMeshContainer->vCustomColorB = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f);
						break;
					}
				}
			}

			lstrcpy(szFullName, m_pPath);
			lstrcat(szFullName, szSpecularName);

			if(FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullName, &pMeshContainer->ppTextureSpecular[i])))
			{
				pMeshContainer->ppTextureSpecular[i] = NULL;
			}
		}
	}
	else
	{
		ZeroMemory(&pMeshContainer->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.a = 1.f;
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
	}

	//스키닝 정보...
	if(pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		//pMeshContainer->pOriMesh = pMesh;

		DWORD dwNumBone = pSkinInfo->GetNumBones();
		DWORD numBoneCombo = 0;

		pSkinInfo->ConvertToIndexedBlendedMesh(pMeshData->pMesh, 
			D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, 
			64, 
			NULL, 
			NULL, 
			NULL, NULL, &pMeshContainer->dwInfluenceMax, &numBoneCombo, &pMeshContainer->pBoneCombinationBuf, &pMeshContainer->MeshData.pMesh);

		pMeshContainer->pFrameOffsetMatrix = new D3DXMATRIX[dwNumBone];
		ZeroMemory(pMeshContainer->pFrameOffsetMatrix, sizeof(D3DXMATRIX) * dwNumBone);

		for(DWORD i = 0; i < dwNumBone; ++i)
		{
			pMeshContainer->pFrameOffsetMatrix[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}

		pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->dwNumAttributeGroups);
		pMeshContainer->AttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->dwNumAttributeGroups];
		pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->AttributeTable, NULL);
	}
	else
	{
		pMeshContainer->pSkinInfo = NULL;
		pMeshContainer->pFrameOffsetMatrix = NULL;
		pMeshContainer->pOriMesh = NULL;
	}

	(*ppNewMeshContainer) = pMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if(pFrameToFree == NULL)
	{
		return E_FAIL;
	}

	if(pFrameToFree->pMeshContainer != NULL)
	{
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}

	if(pFrameToFree->pFrameFirstChild != NULL)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}

	if(pFrameToFree->pFrameSibling != NULL)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	Safe_Delete_Array(pFrameToFree->Name);
	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	DERIVED_MESHCONTAINER* pMeshContainer = static_cast<DERIVED_MESHCONTAINER*>(pMeshContainerToFree);

	for(DWORD i = 0; i < pMeshContainerToFree->NumMaterials; ++i)
	{
		Safe_Release(pMeshContainer->ppTexture[i]);
		Safe_Release(pMeshContainer->ppTextureNormal[i]);
		Safe_Release(pMeshContainer->ppTextureCustom[i]);
		Safe_Release(pMeshContainer->ppTextureSpecular[i]);
	}

	Safe_Delete_Array(pMeshContainer->ppTextureNormal);
	Safe_Delete_Array(pMeshContainer->ppTexture);
	Safe_Delete_Array(pMeshContainer->ppTextureCustom);
	Safe_Delete_Array(pMeshContainer->ppTextureSpecular);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pMeshContainer->ppFrameConbinedMartix);
	Safe_Delete_Array(pMeshContainer->pResourlMatrix);
	Safe_Delete_Array(pMeshContainer->pAdjacency);

	Safe_Release(pMeshContainer->pOriMesh);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pSkinInfo);
	Safe_Release(pMeshContainer->pBoneCombinationBuf);

	Safe_Delete(pMeshContainer->AttributeTable);
	Safe_Delete(pMeshContainer);

	return S_OK;
}

void Engine::CLoader::AllocateName(const CHAR* const pName, CHAR** ppNewName)
{
	int iLength = strlen(pName);

	*ppNewName = new CHAR[iLength + 1];

	strcpy_s(*ppNewName, iLength + 1, pName);
}

bool Engine::CLoader::Find_Name(TCHAR* pFindName, TCHAR* pFileName)
{
	int iFileLength = lstrlen(pFileName);

	for(int i = 0; i < iFileLength; ++i)
	{
		if(pFindName[0] == pFileName[i])
		{
			int iFindLength = lstrlen(pFindName);
			
			TCHAR szFind[20] = L"";

			for(int j = 0; j < iFindLength; ++j)
			{
				szFind[j] = pFileName[i + j];
			}

			if(lstrcmp(szFind, pFindName) == 0)
			{
				return true;
			}
		}
	}

	return false;
}